# -*- coding: utf-8 -*-
# CUDA_VISIBLE_DEVICES=-1 python main.py -c ./configs/config_train_word_half.json

from __future__ import absolute_import, division, print_function, unicode_literals

from data_loader.data_loader import DataLoader
from models.d2gan import DetectionModel
from trainers.trainer import Trainer

from utils import tools
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

	print("Preprocess data")
	train_data, valid_data = data_loader.get_data_online(from_config=True, make_channel=True)

	# MODEL 생성
	print("Create the model")
	model = DetectionModel(config)

	# Trainer 생성
	trainer = Trainer(model, train_data, valid_data, config)

	# Train 시작
	trainer.train_gan()

if __name__ == '__main__':
	main()
