# -*- coding: utf-8 -*-
# CUDA_VISIBLE_DEVICES=-1 python inference.py -c ./configs/config_inference_half.json

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


def main():
    args = get_args()
    config = process_config(args.config)

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
    main()
