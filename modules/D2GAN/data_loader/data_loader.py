# -*- coding: utf-8 -*-

import tensorflow as tf
import tensorflow.keras as keras
from tensorflow.python.framework import dtypes

import os
import numpy as np
import cv2
from PIL import Image
from tqdm import tqdm

import mlflow
import mlflow.keras

from configparser import ConfigParser

path_config_file = os.getcwd().split('docrv2_sroie')[0] + 'docrv2_sroie/' + 'config.ini'
common_variable = ConfigParser()
common_variable.read(path_config_file)

import sys

sys.path.append(common_variable['database']['module_path'])

from db_util import *
from db import *

from base.base_data_loader import BaseDataLoader
from utils import dirs
from utils import image_utils
from utils.image_utils import drawBoxes

DB_MAIN_PATH = common_variable['database']['DB_MAIN_PATH']


class DataLoader(BaseDataLoader):
    def __init__(self, config):
        super(DataLoader, self).__init__(config)
        self.datalen = 0

        # Set main db path
        if config.META_DB.DB_MAIN_PATH != '':
            DB_MAIN_PATH = self.config.META_DB.DB_MAIN_PATH
        print("...create meta db")
        # create meta db
        self.db_meta, self.dbinfofile = self.set_meta_db()

        print("...log to mlflow")
        if config.MLFLOW.module_name != 'None':
            # log meta db to mlflow
            self.log_meta_db_to_mlflow()

    def set_meta_db(self):
        # create new meta db
        path = self.config.META_DB.path
        name = self.config.META_DB.name
        dbinfofile = path + name + self.config.META_DB.log
        desc = self.config.META_DB.description

        # Load meta db
        if self.config.META_DB.load_existing_meta_db is False:
            dirs.create_dirs([path])
            db_meta = create_meta_db(path=path, name=name, description=desc)

            # set wanted db name
            db_list_string = self.config.META_DB.dblist
            db_list = db_list_string.split(',')

            # register data db to meta db
            db_meta = set_dbs_to_meta_db(db_meta=db_meta, db_list=db_list)
        else:
            db_meta = open_env(path=path + name)

        # written to data summary file
        print_env(db_meta, save=True, save_path=dbinfofile, wmode='w')
        print_db(db_meta, save=True, save_path=dbinfofile)

        return db_meta, dbinfofile

    def log_meta_db_to_mlflow(self):
        db_meta = self.db_meta

        # activate mlflow run
        mlflow.ActiveRun(self.config.mlrun)

        # get/set meta db info
        path = self.config.META_DB.path
        name = self.config.META_DB.name

        list_db_name, list_db_ref, list_db_env = get_dbs_from_meta_db(env=db_meta)
        envsm_main = get_env_summary(db_meta, prefix='__meta__/')
        mlflow.log_params(envsm_main)
        mlflow.log_param('__meta__/-', '-' * 200)

        # get/set data db info
        for idx in range(len(list_db_name)):
            envsm = get_env_summary(list_db_env[idx], prefix='_' + list_db_name[idx] + '/')
            mlflow.log_params(envsm)

            mlflow.log_param('_' + list_db_name[idx] + '/-', '-' * 200)

            print_env(list_db_env[idx], save=True, save_path=self.dbinfofile)

        # register metadb artifact
        mlflow.log_artifact(path + name)
        with open(self.dbinfofile, 'r') as file:
            data_info_string = file.read().replace('\n', '')

        mlflow.set_tag('data_summary', data_info_string)

    def get_data_online(self, batch_size=8, size=512, distanceRatio=[1.5, 1], sratio=[5, 1, 1, 5], amp=[1, 1],
                        descale=2, make_channel=False, from_config=False, use_otsu=False):
        # get data from config
        if from_config == True:
            size = self.config.data.mask_sampling_size
            distanceRatio = np.array(self.config.data.mask_dist.split(','), dtype=np.float32)
            sratio = np.array(self.config.data.mask_asym_ratio.split(','), dtype=np.float32)
            amp = np.array(self.config.data.mask_amp.split(','), dtype=np.float32)
            descale = self.config.model.descale_factor
            use_otsu = self.config.data.otsu

        # create meta db
        db_meta = self.db_meta

        # create heatmap
        heatmap = image_utils.get_asymmetric_gaussian_heatmap(size, distanceRatio[0], sx=sratio[0], sy=sratio[1],
                                                              amp=amp[0])
        heatmap_affinity = image_utils.get_asymmetric_gaussian_heatmap(size, distanceRatio[1], sx=sratio[2],
                                                                       sy=sratio[3], amp=amp[1])

        # set params
        inputsize = (self.config.data.image_size_h, self.config.data.image_size_w)
        batch_size = self.config.trainer.batch_size
        epoch_size = self.config.trainer.num_epochs
        step_size = self.config.trainer.num_steps
        train_valid_ratio = self.config.trainer.train_valid_ratio

        # get data list and char coordinates
        img_list = read_all_keys_from_meta_db(db_meta, 'img-', remove_last=True)
        char_list = read_all_keys_from_meta_db(db_meta, 'char_c', remove_last=True)

        # merge list
        imgs = []
        chars = []
        for idx in range(len(img_list)):
            imgs.append(img_list[idx][0] + ',' + img_list[idx][1])
            chars.append(char_list[idx][0] + ',' + char_list[idx][1])

        # split dataset train/validation
        img_s = split_list(imgs,
                           [train_valid_ratio, 1 - train_valid_ratio],
                           shuffle=True,
                           data_length=[epoch_size * step_size * batch_size, epoch_size * batch_size])
        coord_s = split_list(chars,
                             [train_valid_ratio, 1 - train_valid_ratio],
                             shuffle=True,
                             data_length=[epoch_size * step_size * batch_size, epoch_size * batch_size])

        if make_channel == True:
            convChannel = 3
        else:
            convChannel = 2

        def make_tfdataset(imgs, coods):
            no_data = len(coods)

            def get_data_from_addr(imgaddr, lineaddr):
                # parsing data address
                imgaddr = imgaddr.numpy().decode()
                lineaddr = lineaddr.numpy().decode()
                imgaddr = imgaddr.split(',')
                lineaddr = lineaddr.split(',')

                # read data
                denv = open_env(DB_BASE + imgaddr[0])
                dkey = imgaddr[1]
                lkey = lineaddr[1]
                db = denv.open_db(b'db_data')
                with denv.begin(write=False) as txn:
                    db_cs = txn.cursor(db)
                    img = decode_img(db_cs.get(str(dkey).encode()))
                    img = np.array(img)
                    tlines = decode_pd(db_cs.get(str(lkey).encode()),
                                       sep=get_value_by_key(denv, 'sep'))

                # process line
                lines = [None] * len(tlines)
                for i in range(len(tlines)):
                    for j in range(8):
                        tlines[i][j] = int(tlines[i][j])
                    lines[i] = tuple(tlines[i])

                if use_otsu:
                    img = cv2.cvtColor(img, cv2.COLOR_RGB2GRAY)
                    ret, img = cv2.threshold(img, 0, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU)
                    img = cv2.cvtColor(img, cv2.COLOR_GRAY2RGB)

                imap = image_utils.compute_maps(heatmap=heatmap,
                                                image_height=img.shape[0],
                                                image_width=img.shape[1],
                                                lines=[lines], heatmap_affinity=heatmap_affinity,
                                                descale=descale, make_channel=make_channel)
                imap = imap - 0.5

                return img, imap

            def preprocess(imgaddr, lineaddr):
                # get data
                img, imap = tf.py_function(get_data_from_addr, [imgaddr, lineaddr], [tf.float32, tf.float32])

                # normalize images
                img_array = image_utils.per_image_standardization(img)
                imap_array = image_utils.per_image_standardization_map(imap)

                # set output tensor shape (or keras fit function makes error)
                img_array.set_shape([None, None, 3])
                imap_array.set_shape([None, None, convChannel])
                return img_array, imap_array

            return tf.data.Dataset.from_tensor_slices((imgs, coods)).map(preprocess,
                                                                         num_parallel_calls=tf.data.experimental.AUTOTUNE).batch(
                batch_size).prefetch(
                tf.data.experimental.AUTOTUNE)

        dataset_train = make_tfdataset(img_s[0], coord_s[0])
        dataset_valid = make_tfdataset(img_s[1], coord_s[1])

        return dataset_train, dataset_valid

    def get_inference_data_online(self, use_maindb=True):
        # create meta db
        db_meta = self.db_meta
        config = self.config
        batch_size = config.trainer.batch_size

        # get data list and char coordinates
        img_list = read_all_keys_from_meta_db(db_meta, 'img-')

        self.datalen = len(img_list)

        # convert to tuple
        # merge list
        imgs = []
        for idx in range(len(img_list)):
            imgs.append(img_list[idx][0] + ',' + img_list[idx][1])

        def make_tfdataset(imgs):
            def get_data_from_addr(imgaddr):
                # parsing data address
                imgaddr = imgaddr.numpy().decode()
                imgaddr = imgaddr.split(',')

                # read data
                if use_maindb is True:
                    denv = open_env_read(path=imgaddr[0], base=True)
                else:
                    denv = open_env_read(path=os.path.join(self.config.META_DB.path, imgaddr[0]), base=False)
                dkey = imgaddr[1]

                db = denv.open_db(str('db_data').encode())
                with denv.begin(write=False) as txn:
                    db_cs = txn.cursor(db)
                    img = decode_img(db_cs.get(str(dkey).encode()))
                    img = np.array(img, dtype=np.uint8)
                return img

            def preprocess(imgaddr):
                # get data
                img = tf.py_function(get_data_from_addr, [imgaddr], [tf.uint8])

                # normalize data
                img_array = tf.image.convert_image_dtype(img, dtypes.float32)
                img_array = tf.image.per_image_standardization(img_array)

                # set output tensor shape (or keras fit function makes error)
                img_array.set_shape([1, None, None, 3])

                return img_array

            def preprocess_raw(imgaddr):
                # get_data
                img = tf.py_function(get_data_from_addr, [imgaddr], [tf.uint8])

                # set output tensor shape (or keras fit function makes error)
                img_array = tf.image.convert_image_dtype(img, dtypes.uint8)
                img_array.set_shape([1, None, None, 3])

                return img_array

            return tf.data.Dataset.from_tensor_slices(imgs).map(preprocess_raw,
                                                                num_parallel_calls=tf.data.experimental.AUTOTUNE)

        return make_tfdataset(imgs)
