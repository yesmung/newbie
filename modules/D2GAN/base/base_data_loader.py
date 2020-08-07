# -*- coding: utf-8 -*-

class BaseDataLoader(object):
	def __init__(self, config):
		self.config = config

	def get_train_data(self):
		raise NotImplementedError

	def get_validation_data(self):
		raise NotImplementedError

	def get_test_data(self):
		raise NotImplementedError