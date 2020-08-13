# -*- coding: utf-8 -*-
# CUDA_VISIBLE_DEVICES=1 python main.py -c ./configs/config_train.json
# nohup OMP_NUM_THREADS=12 CUDA_VISIBLE_DEVICES=3 python main.py -c ./configs/config_usecase_1_attn_l_usecharonly_additions2_2.json
# mlflow ui -p5051 --backend-store-uri sqlite:////home/dk/docr2/mlflow/mlflow.db --default-artifact-root /home/dk/docr2/mlflow/
from __future__ import absolute_import, division, print_function, unicode_literals

from data_loader.data_loader import DataLoader
from trainers.trainer import Trainer
from utils.config import optTransplator
from utils.args import get_args
from utils.config import process_config
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

	# Input parameter parsing to opt format
	opt, config = optTransplator(config)
	print(opt)
	print(config)

	# Trainer 생성
	print("Start training the model.")
	trainer = Trainer(opt, data_loader, config)
	trainer.train()

if __name__ == '__main__':
	main()