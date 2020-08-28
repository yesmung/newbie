# -*- coding: utf-8 -*-

import tensorflow as tf
import tensorflow.keras as keras
from tensorflow.python.framework import dtypes

import os
import numpy as np
import cv2
import six
from tqdm import tqdm
from PIL import Image

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
from utils.image_utils import drawBoxes, per_image_standardization, per_image_destandardization

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

    def get_data_online(self, batch_size=8, descale=2, from_config=False):
        # get data from config
        if from_config == True:
            descale = self.config.model.descale_factor

        # create meta db
        db_meta = self.db_meta

        # set params
        inputsize = (self.config.data.image_size_h, self.config.data.image_size_w)
        batch_size = self.config.trainer.batch_size
        epoch_size = self.config.trainer.num_epochs
        step_size = self.config.trainer.num_steps
        train_valid_ratio = self.config.trainer.train_valid_ratio

        # get data list and char coordinates
        print("... read keys")
        img_list = read_all_keys_from_meta_db(db_meta, 'img-')

        # merge list
        img_input = []
        img_target = []
        flag = 0
        for idx in range(len(img_list)):
            if idx % 2 == 0:
                img_input.append(img_list[idx][0] + ',' + img_list[idx][1])
            if idx % 2 == 1:
                img_target.append(img_list[idx][0] + ',' + img_list[idx][1])

        # Split dataset train/validation
        print("... split list")
        img_input_s = split_list(img_input,
                                 [train_valid_ratio, 1 - train_valid_ratio],
                                 shuffle=True,
                                 data_length=[epoch_size * step_size * batch_size, epoch_size * batch_size])
        img_target_s = split_list(img_target,
                                  [train_valid_ratio, 1 - train_valid_ratio],
                                  shuffle=True,
                                  data_length=[epoch_size * step_size * batch_size, epoch_size * batch_size])

        def make_tfdataset(imgs, coods):
            no_data = len(coods)

            def get_data_from_addr(imgaddr1, imgaddr2):
                # parsing data address
                imgaddr1 = imgaddr1.numpy().decode()
                imgaddr1 = imgaddr1.split(',')
                imgaddr2 = imgaddr2.numpy().decode()
                imgaddr2 = imgaddr2.split(',')

                # read data
                denv = open_env(DB_BASE + imgaddr1[0])
                dkey1 = imgaddr1[1]
                dkey2 = imgaddr2[1]
                db = denv.open_db(b'db_data')
                with denv.begin(write=False) as txn:
                    db_cs = txn.cursor(db)
                    img1 = decode_img(db_cs.get(str(dkey1).encode()))
                    img1 = np.array(img1)

                    # create random faint image
                    if np.random.randint(0, high=5, size=1)[0] == 1:
                        bufflow = six.BytesIO()
                        img1 = Image.fromarray(np.array(img1, dtype=np.uint8))
                        img1.save(bufflow, 'JPEG', quality=int(np.random.randint(1, high=20, size=1)[0]))
                        img1 = Image.open(bufflow).convert('RGB')
                        img1 = np.array(img1)
                        # print('...faintcorr:', faintcorr)

                    # add faint image option
                    lench = np.random.randint(0, high=7, size=1)[0]
                    if lench == 1:
                        faintcorr = np.random.randint(20, high=50, size=1)[0]
                        img1 = np.array(img1, dtype=np.float32) + faintcorr
                        img1 = np.clip(img1,0,255)

                    elif lench == 2:
                        faintcorr = np.random.randint(20, high=50, size=1)[0]
                        img1 = np.array(img1, dtype=np.float32) - faintcorr
                        img1 = np.clip(img1, 0, 255)

                    img2 = decode_img(db_cs.get(str(dkey2).encode()))
                    img2 = np.array(img2)
                    # img1 = tf.image.resize(img1, [256,256])
                    # img2 = tf.image.resize(img2, [128,128])

                return img1, img2

            def preprocess(imgaddr, lineaddr):
                # get data
                img, imap = tf.py_function(get_data_from_addr, [imgaddr, lineaddr], [tf.float32, tf.float32])

                # normalize images
                img_array = image_utils.per_image_standardization(img)
                imap_array = image_utils.per_image_standardization_map(imap)

                # set output tensor shape (or keras fit funtion makes error)
                img_array.set_shape([None, None, 3])
                imap_array.set_shape([None, None, 3])

                return img_array, imap_array

            return tf.data.Dataset.from_tensor_slices((imgs, coods)).map(preprocess,
                                                                         num_parallel_calls=tf.data.experimental.AUTOTUNE).batch(
                batch_size).prefetch(
                tf.data.experimental.AUTOTUNE)

        print('... make tf data : train')
        dataset_train = make_tfdataset(img_input_s[0], img_target_s[0])
        print('... make tf data : valid')
        dataset_valid = make_tfdataset(img_input_s[1], img_target_s[1])

        return dataset_train, dataset_valid

    def get_inference_data(self):
        # create meta db
        db_meta = self.db_meta

        imgs = read_all_data_from_meta_db(db_meta, 'img')

        """
        # resize image
        for i, im in enumerate(imgs):
            imsize = im.size
            imgs[i] = im.resize((imsize[0]//2,imsize[1]//2))
        """
        image_array = img2array(imgs)

        dataset_inference = []
        for el in image_array:
            # dataset_inference.append(cv2.cvtColor(cv2.cvtColor(el, cv2.COLOR_RGB2GRAY), cv2.COLOR_GRAY2RGB))
            dataset_inference.append(el)
        return dataset_inference

    def save_to_db(self, prediction=None, data=None):
        # save to database
        config = self.config
        db_meta = self.db_meta

        # get label / reference data
        refs = read_all_data_from_meta_db(db_meta, 'ref')
        labels = read_all_data_from_meta_db(db_meta, 'label')

        path = config.inference.data_db_path
        name = config.inference.data_db_name
        description = config.inference.data_db_description

        # DB 불러오기
        if config.inference.create_new_data_db == True:
            # 빈 data db 생성
            datadb = create_data_db(path=path, name=name, description=description, data_class='INFERENCE')
        else:
            datadb = open_env(path + name)

        # db 에 data 저장
        no_data = len(data)
        for imgidx in tqdm(range(no_data)):
            theimage = data[imgidx]
            pred = per_image_destandardization(prediction[imgidx][0,])

            # update to db
            update_data(datadb, index=imgidx, label=labels[imgidx],
                        image=Image.fromarray(np.array(pred, dtype=np.uint8)), ref=refs[imgidx])

        # DATA DB 시간 업데이트
        update_time_stemp(datadb)
        print_env(datadb)

        # MAIN META DB에 DATA DB 정보 반영 (REGISTER)
        db_main = register_db(env=db_meta, db=datadb)
