# -*- coding: utf-8 -*-

import tensorflow as tf
import tensorflow.keras as keras
from tensorflow.keras import preprocessing
from tensorflow.keras.utils import to_categorical

import os
import numpy as np
import cv2
import json
from tqdm import tqdm

import mlflow
import mlflow.keras

import sys
sys.path.append('/media/myungsungkwak/msdisk/docrv2_sroie/modules/database')
from db_util import *
from db import *

from base.base_data_loader import BaseDataLoader

class DataLoader(BaseDataLoader):
	def __init__(self, config):
		super(DataLoader, self).__init__(config)

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
		self.dbinfofile = dbinfofile

		description = self.config.META_DB.description

		# Load meta db
		if self.config.META_DB.load_existing_meta_db is False:
			dirs.create_dirs([path])
			db_meta = create_meta_db(path=path, name=name, description=description)

			# set wanted db name
			db_list_string = self.config.META_DB.dblist
			db_list = db_list_string.split(',')

			# register data db to meta db
			db_meta = set_dbs_to_meta_db(db_meta=db_meta, db_list=db_list)
		else:
			db_meta = open_env(path=path+name)

		# written to data summary file
		print_env(db_meta, save=True, save_path=dbinfofile, wmode='w')
		print_db(db_meta, save=True, save_path=dbinfofile)

		return db_meta

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
		mlflow.log_params('__meta__/-','-'*200)

		# get/set data db info
		for idx in range(len(list_db_name)):
			envsm = get_env_summary(list_db_env[idx], prefix='_'+list_db_name[idx]+'/')
			mlflow.log_params(envsm)
			dbsm = get_db_summary(list_db_env[idx], prefix='_'+list_db_name[idx]+'/')
			mlflow.log_params(dbsm)
			mlflow.log_param('_'+list_db_name[idx]+'/-','-'*200)

			print_env(list_db_env[idx], save=True, save_path=dbinfofile)
			print_db_summary(list_db_env[idx], save=True, save_path=dbinfofile)

		# register metadb artifact
		mlflow.log_artifact(path+name)
		with open(dbinfofile, 'r') as file:
			data_info_string = file.read().replace('\n', '')

		mlflow.set_tag('data_summary', data_info_string)

	def get_data_process_online(self):
		# create meta db
		db_meta = self.db_meta

		# set params
		inputsize = (self.config.data.image_size_h, self.config.data.image_size_w)
		batch_size = self.config.trainer.batch_size
		epoch_size = self.config.trainer.num_epochs
		step_size = self.config.trainer.num_steps
		train_valid_ratio = self.config.trainer.train_valid_ratio

		maxlen = self.config.data.max_len

		# init random seed
		np.random.seed(0)

		# Load data into RAM
		print("... load text data into RAM")
		texts = read_all_data_from_meta_db(db_meta, 'text')
		labels = read_all_data_from_meta_db(db_meta, 'label')

		# convert images
		# [label_int, label_cat] = label2int(labels)
		text_array = np.array(texts)
		label_array = np.array(labels, dtype=np.int)
		labelmax = np.max(label_array)+1

		# split data
		print("... split list")
		label_s = split_list(label_array, [train_valid_ratio, 1-train_valid_ratio], shuffle=True, data_length=[epoch_size*step_size*batch_size, epoch_size*batch_size])
		txt_s = split_list(text_array, [train_valid_ratio, 1-train_valid_ratio], shuffle=True, data_length=[epoch_size*step_size*batch_size, epoch_size*batch_size])

		# make train, test dataset
		train_x = txt_s[0]
		train_y = to_categorical(label_s[0])

		test_x = txt_s[1]
		test_y = to_categorical(label_s[1])

		# make tokenizer
		if self.config.data.use_tokenizer is True:
			print("...... load tokenizer from = ",self.config.data.tokenizer_path)
			with open(self.config.data.tokenizer_path, 'r') as f:
				tokenizer = preprocessing.text.tokenizer_from_json(json.load(f))
		else:
			print("...... create tokenizer")
			train_x_chr = list(map(lambda e: list(e), train_x))
			tokenizer = preprocessing.text.Tokenizer(oov_token='<UNK>')
			tokenizer.fit_on_texts(train_x_chr)

		"""
		def preprocess(lst):
			lst_chr = list(map(lambda e : list(e), lst))
			lst_sequences = tokenizer.texts_to_sequences(lst_chr)
			padded_lst_sequences = preprocessing.sequence.pad_sequences(lst_sequences, maxlen=15, padding='post')
			return padded_lst_sequences

		print("...... preprocess data")
		# 매우 오래걸림 매우
		padded_train_x_sequences = preprocess(train_x)
		padded_test_x_sequences = preprocess(text_x)
		"""

		# upload tokenizer to mlflow
		fpath = self.config.META_DB.path + self.config.META_DB.name + '/tokenizer'
		os.makedirs(fpath, exist_ok=True)
		with open(fpath + '/tokenizer.json', 'w') as f:
			json.dump(tokenizer.to_json(), f)
		mlflow.log_artifact(fpath)

		# create tf dataset
		maxlen = self.config.data.max_len
		nocat = self.config.data.num_categories

		def make_tfdataset(rtext, rlabel):
			def preprocess(lst, label):
				lst = lst.numpy().decode()
				label = label.numpy()

				lst_chr = list(map(lambda e: list(e), [lst]))
				lst_sequences = tokenizer.texts_to_sequences(lst_chr)
				padded_lst_sequences = preprocessing.sequence.pad_sequences(lst_sequences, maxlen=maxlen, padding='post')[0]

				ltext_array = np.array(padded_lst_sequences, dtype=np.float32)
				llabel_array = np.array(label, dtype=np.float32)

				return ltext_array, llabel_array

			def mapfunction(itext, ilabel):
				ltext_array, llabel_array = tf.py_function(preprocess, [itext, ilabel], [tf.float32, tf.float32])

				ltext_array.set_shape([maxlen])
				llabel_array.set_shape([nocat])

				return ltext_array, llabel_array

			return tf.data.Dataset.from_tensor_slices((rtext, rlabel)).map(mapfunction, num_parallel_calls=tf.data.experimental.AUTOTUNE).batch(batch_size).prefetch(
				tf.data.experimental.AUTOTUNE)

		print("...... create tfdatasets")
		dataset_train = make_tfdataset(train_x, train_y)
		dataset_valid = make_tfdataset(test_x, test_y)

		return dataset_train, dataset_valid, tokenizer

	def get_inference_data(self):
		# create meta db
		db_meta = self.db_meta
		config = self.config
		target_prefix = config.META_DB.target_prefix
		batch_size = config.trainer.batch_size

		# Load data infto RAM
		print("......load text data into RAM")
		texts = read_all_data_from_meta_db(db_meta, target_prefix)
		text_array = []
		data_index = []
		for ii, txt in enumerate(texts):
			text_array.extend(np.array(txt))
			data_index.extend(np.ones(len(txt), dtype=int)*ii)
		# convert to array
		# text_array = np.array(texts)

		# get tokenizer
		if self.config.data.use_tokenizer is True:
			print("......load tokenizer from = ", self.config.data.tokenizer_path)
			with open(self.config.data.tokenizer_path, 'r') as f:
				tokenizer = preprocessing.text.tokenizer_from_json(json.load(f))
		else:
			raise Exception("No tokenizer available. please provide tokenizer path from config file.")

		maxlen = self.config.data.max_len

		def preprocess(word_list):
			lst_chr = list(map(lambda e: list(e), word_list))
			lst_sequences = tokenizer.texts_to_sequences(lst_chr)
			padded_lst_sequences = preprocessing.sequence.pad_sequences(lst_sequences, maxlen=maxlen, padding='post')
			return padded_lst_sequences

		print("......preprocess data")
		padded_text_array = preprocess(text_array)

		def make_tfdataset(ltext):
			class lmdbdataset(tf.data.Dataset):
				def _generator():
					for el in ltext:
						yield el

				def __new__(cls):
					return tf.data.Dataset.from_generator(
							cls._generator,
							output_types=(tf.float32),
							output_shapes=([maxlen])
						)
			return lmdbdataset().batch(batch_size).prefetch(tf.data.experimental.AUTOTUNE)

		print("......create tfdatasets")
		return make_tfdataset(padded_text_array), [text_array, data_index], tokenizer

	def save_to_db(self, prediction=None, data=None):
		# save to database
		config = self.config

		# DB load
		path = config.inference.data_db_path
		name = config.inference.data_db_name
		if config.inference.create_new_data_db == True:
			# 빈 data db 생성
			description = config.inference.data_db_description
			datadb = create_data_db(path=path, name=name, description=description, data_class='INFERENCE')
		else:
			datadb = open_env(path+name)
			print_env(datadb)

		alist = config.data.categories
		alist = alist.split(',')
		anum = list(range(0, len(alist)))
		answer = dict(zip(anum, alist))

		def get_ens(data):
			return answer[np.argmax(data)]

		# get answers saving to db
		nedlist = [get_ans(di) for di in prediction]

		# save to db
		for idx in list(set(data[1])):
			wordner = [nedlist[ii] for ii, aidx in enumerate(data[1]) if aidx == idx]
			update_data(datadb, index=idx, label=None, word_ner=wordner)