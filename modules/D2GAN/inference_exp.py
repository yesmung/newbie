# -*- coding: utf-8 -*-
# CUDA_VISIBLE_DEVICES=-1 python inference_exp.py -c ./configs/config_inference_rank.json

from __future__ import absolute_import, division, print_function, unicode_literals

from data_loader.data_loader import DataLoader
from models.d2gan import DetectionModel
from trainers.trainer import Trainer

from utils import tools
from utils.args import get_args
from utils.config import process_config
from utils.dirs import create_dirs

import os
import numpy as np

# os.environ["CUDA_VISIBLE_DEVICES"] = "-1"


def main(models=None):
    #for idx, modelpath in enumerate(models):
    for idx in range(6,len(models)):
        modelpath = models[idx]
        args = get_args()
        config = process_config(args.config, modelpath=modelpath, savedbname='exp_result_%03d'%idx)

        # META DB 생성
        print("Create META DB and load data")
        data_loader = DataLoader(config)

        # MODEL 생성
        print("Load the model")
        model = DetectionModel(config)

        # PREDICT (db)
        print("Perform predictions and save to DB")
        db, coordinates_only_table_word_list = model.inference_and_save_to_db(data_loader, save_image=True)

        # print("Save coordinates as text files and archiving in to {}".format(config.inference.sroie_path))
        # model.save_to_sroie(predictions=coordinates_only_table_word_list, db_meta=data_loader.db_meta)


if __name__ == '__main__':
    infmodellist = ['/home/dk/docrv2_sroie/mlflow/4a39f96af52d441e8133e444d72ff3ac/artifacts/models_generator/data/model.h5',
                    '/home/dk/docrv2_sroie/mlflow/0646a810d12b4b55ae5303c06d0ddf78/artifacts/models_generator/data/model.h5',
                    '/home/dk/docrv2_sroie/mlflow/6d8984bd1cf54a3d8f2ef1036a2e55fa/artifacts/models_generator/data/model.h5',
                    '/home/dk/docrv2_sroie/mlflow/ddd22ddfacb840ceaf42eec82f2d611f/artifacts/models_generator/data/model.h5',
                    '/home/dk/docrv2_sroie/mlflow/ad9bb11ba7cf4374a57b8a2dec2a36a2/artifacts/models_generator/data/model.h5',
                    '/home/dk/docrv2_sroie/mlflow/4d1c49f0318e483f864c7c96e09e1193/artifacts/models_generator/data/model.h5',
                    '/home/dk/docrv2_sroie/mlflow/47596ff6b96542eaad49e3a63179acd2/artifacts/models_generator/data/model.h5',
                    '/home/dk/docrv2_sroie/mlflow/7695454be27348938a906f003d572915/artifacts/models_generator/data/model.h5',
                    '/home/dk/docrv2_sroie/mlflow/7b0255c866d5474c9306a4240e8a1892/artifacts/models_generator/data/model.h5',
                    '/home/dk/docrv2_sroie/mlflow/ecbb7939246b4f45bd23b0b6341716a2/artifacts/models_generator/data/model.h5',
                    '/home/dk/docrv2_sroie/mlflow/c32f1155342e49d2a442d0030d272be7/artifacts/models_generator/data/model.h5']
    infmodellist = infmodellist[::-1]

    main(models = infmodellist)
