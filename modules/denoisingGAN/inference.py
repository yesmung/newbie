# -*- coding: utf-8 -*-
# OMP_NUM_THREADS=2 CUDA_VISIBLE_DEVICES=-1 python inference.py -c ./configs/config_inference.json

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


def main():
    args = get_args()
    config = process_config(args.config)

    # META DB 생성
    print("Create META DB and load data")
    data_loader = DataLoader(config)
    dataset = data_loader.get_inference_data()

    # MODEL 생성
    print("Load the model")
    model = DetectionModel(config)

    # PREDICT
    print("Perform predictions")
    predictions_img = model.detect(dataset, descale=config.model.descale_factor)

    # 결과 저장 to Inference DB
    print("Save to DB")
    data_loader.save_to_db(prediction=predictions_img, data=dataset)


if __name__ == '__main__':
    main()
