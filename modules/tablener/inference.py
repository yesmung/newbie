# -*- coding: utf-8 -*-
# CUDA_VISIBLE_DEVICES=1 python main.py -c ./configs/config_train.json

from __future__ import absolute_import, division, print_function, unicode_literals

from data_loader.data_loader import DataLoader
from models.model import Model
from trainers.trainer import Trainer

from utils.args import get_args
from utils.config import process_config
from utils.dirs import create_dirs
from utils.mlflow import start_mlflow

import os
import numpy as np

def main():
	args = get_args()
	config = process_config(args.config)

	# META DB 생성
	print("Create the data generator")
	data_loader = DataLoader(config)
	data = data_loader.get_inference_data()

	# MODEL 생성
	print("Create the model.")
	model = Model(config, data[2])

	# Trainer 생성
	print("Create the trainer.")
	trainer = Trainer(model.model, data[0], config)

	# Inference 시작
	print("Start training the model.")
	predictions = trainer.inference()

	# 결과 저장
	# print("Save data as npy format")
	# np.save('test.npy', [predictions, data[1]], allow_pickle=True)

	# Save data to DB
	print("Save data to DB")
	data_loader.save_to_db(prediction=predictions, data=data[1])

if __name__ == '__main__':
	main()