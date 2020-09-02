# -*- coding: utf-8 -*-
# CUDA_VISIBLE_DEVICES=1 python inference.py -c ./configs/config_inference.json

from __future__ import absolute_import, division, print_function, unicode_literals

from data_loader.data_loader import DataLoader
from trainers.trainer import Trainer
from utils.config import process_config
from utils.config import optTransplator

from utils.args import get_args
from utils.mlflow import start_mlflow
import os
import numpy as np

def main():
	args = get_args()
	config = process_config(args.config)

	# META DB 생성
	print("Create the data generator")
	data_loader = DataLoader(config)

	# Input parameter parsing to opt format
	opt, config = optTransplator(config)
	print(opt)
	print(config)

	# Trainer 생성
	print("Start inference the model.")
	trainer = Trainer(opt, data_loader, config)
	predictions = trainer.inference()

	# save data to db
	print("Save data to DB")
	data_loader.save_to_db(predictions=predictions)

	#print("Save data as SROIE format")
	#data_loader.save_to_sroie(predictions=predictions)

if __name__ == '__main__':
	main()