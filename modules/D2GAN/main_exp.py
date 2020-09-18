# -*- coding: utf-8 -*-
# CUDA_VISIBLE_DEVICES=-1 python main.py -c ./configs/config_train_word_1.json

from __future__ import absolute_import, division, print_function, unicode_literals

from data_loader.data_loader import DataLoader
from models.d2gan import DetectionModel
from trainers.trainer import Trainer

from utils import tools
from utils.args import get_args
from utils.config import process_config
from utils.dirs import create_dirs
from utils.mlflow import start_mlflow
from glob import glob
import os
import numpy as np
import mlflow

def main(dbnames=None, testrange=None):
	for ridx in testrange:
		# try:
		args = get_args()
		dbname = str(dbnames[0:ridx]).replace('[', '').replace(']', '').replace('\'', '').replace(' ', '')
		print(dbname)
		config = process_config(args.config, dbname=dbname)

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
		# except:
		# 	print('err:',ridx)

		# end mlflow
		mlflow.end_run()


if __name__ == '__main__':
	dbnames = glob('/home/dk/docrv2_sroie/DB/d2gan_exp_singleim_*')
	dbnames = list(np.sort([os.path.basename(dbname) for dbname in dbnames]))
	dbnames2 = glob('/home/dk/docrv2_sroie/DB/d2gan_exp_10im_*')
	dbnames2 = list(np.sort([os.path.basename(dbname) for dbname in dbnames2]))
	dbnames.extend(dbnames2)

	testrange = list(range(2, 10))
	testrange.extend(list(range(10, len(dbnames), 5)))
	testrange.append(len(dbnames))
	# run main
	main(dbnames=dbnames, testrange=testrange)
