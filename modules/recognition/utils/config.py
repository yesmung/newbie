# -*- coding: utf-8 -*-

import os
import time
import json
from dotmap import DotMap
from easydict import EasyDict

def get_config_from_json(json_file):
	"""
	Get the config from a json file
	:param json_file
	:return: config(namespace) or config(dictionary)
	"""

	# parse the configurations from the config json file provided
	with open(json_file, 'r') as config_file:
		config_dict = json.load(config_file)

	# convert the dictionary to a namespace using bunch lib
	config = DotMap(config_dict)

	config.json_file = json_file
	config.json_string = str(config_dict)
	config.dict = getKeyValuePair(config_dict)

	return config, config_dict

def process_config(json_file):
	config, _ = get_config_from_json(json_file)
	config.callbacks.tensorboard_log_dir = os.path.join("experiments", time.strftime("%Y-%m-%d", time.localtime()), config.exp.name, "logs/")
	config.callbacks.checkpoint_dir = os.path.join("experiments", time.strftime("%Y-%m-%d", time.localtime()), config.exp.name, "checkpoints/")
	return config

def getKeyValuePair(dic, master_dic={}, master_key=None):
	keys = list(dic.keys())

	for key in keys:
		if type(dic[key]) == dict:
			getKeyValuePair(dic[key], master_dic=master_dic, master_key=key)
		else:
			if master_key == None:
				master_dic[key] = dic[key]
			else:
				master_dic['Config.'+str(master_key)+'.'+str(key)] = dict[key]
	return master_dic

def optTransplator(config):
	try:
		cudasetting = config.model.gpu
	except:
		cudasetting = True

	opt = EasyDict({'experiment_name':config.exp.name,
					'train_data':'',
					'valid_data':config.META_DB.dbvalid,
					'manualSeed':1111,
					'workers':config.data.num_reading_workers,
					'batch_size':config.trainer.batch_size,
					'num_iter':config.trainer.num_epochs,
					'valInterval':config.trainer.valid_interval,
					'continue_model':config.model.pretrained_model_path,
					'saved_model':config.model.pretrained_model_path,
					'adam':False,
					'lr':config.model.learning_rate,
					'beta1':config.model.beta1,
					'rho':config.model.rho,
					'eps':config.model.eps,
					'grad_clip':config.model.grad_clip,
					'select_data':config.META_DB.dblist,
					'target_prefix':config.META_DB.target_prefix,
					'batch_ratio':config.META_DB.batch_ratio,
					'total_data_usage_ratio':1,
					'batch_max_length':config.data.batch_max_length,
					'imgH':config.data.image_size_h,
					'imgW':config.data.image_size_w,
					'rgb':False,
					'character':config.data.character,
					'PAD':config.data.PAD,
					'data_filtering_off':True,
					'Transformation':config.model.transformation,
					'FeatureExtraction':config.model.featureExtraction,
					'SequenceModeling':config.model.sequenceModeling,
					'Prediction':config.model.prediction,
					'num_fiducial':config.model.num_fiducial,
					'input_channel':config.model.fe_input_channel,
					'output_channel':config.model.fe_output_channel,
					'hidden_size':config.model.bl_hidden_size,
					'cuda':cudasetting,
					'num_gpu':1})

	if config.model.optimizer == 'Adam':
		opt.adam = True
	if config.data.character == 'usecase1':
		opt.character = """"Q"""
	elif config.data.character == 'default':
		opt.character = """\t !"!@#$%^&*()+,.\-*/?0123456789:;[]{}|ABCDEFGHIJKLMNOPQRSTUVWXYZ'abcdefghijklmnopqrstuvwxyz"""

	return opt, config