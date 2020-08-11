# -*- coding: utf-8 -*-

class BaseTrain(object):
	def __init__(self, opt, data_loader):
		self.opt = opt
		self.data_loader = data_loader

	def train(self):
		raise NotImplementedError