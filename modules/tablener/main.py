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

def main():
	args = get_args()
	config = process_config(args.config)

	# MLFLOW 시작
	print("Start logging to mlflow")
	config = start_mlflow(config)

	# META DB 생성
	print("Create the data generator")
	data_loader = DataLoader(config)
	print("...load data")
	data = data_loader.get_data_process_online()

	# MODEL 생성
	print("Create the model.")
	model = Model(config, data[2])

	# Trainer 생성
	print("Create the trainer.")
	trainer = Trainer(model.model, data, config)

	# Train 시작
	print("Start training the model.")
	trainer.train()

	# history 출력, detail to MLFLOW
	print("Print history")
	print(trainer.history.history)

if __name__ == '__main__':
	main()