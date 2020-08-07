# -*- coding: utf-8 -*-

from base.base_model import BaseModel

import tensorflow as tf
import tensorflow.keras as keras
from tensorflow.python.framework import dtypes

from tensorflow.keras.layers import Input, Dense, Reshape, Flatten, Dropout, Concatenate
from tensorflow.keras.layers import BatchNormalization, Activation, ZeroPadding2D, Add
from tensorflow.keras.layers import PReLU, LeakyReLU
from tensorflow.keras.layers import UpSampling2D, Conv2D

from tensorflow.keras.applications import VGG19
from tensorflow.keras.models import Sequential, Model
from tensorflow.keras.optimizers import Adam

# mlflow lib
import mlflow
import mlflow.keras

import os
import sys
import typing
import numpy as np
import scipy
import matplotlib.pyplot as plt
import cv2

from PIL import Image
from tqdm import tqdm

# from craft-keras
from denoisingGAN.utils import tools
from denoisingGAN.utils import image_utils

sys.path.append('/media/myungsungkwak/msdisk/docrv2_sroie/modules/database')
from db_util import *
from db import *

AUTOTUNE = tf.data.experimental.AUTOTUNE

class DetectionModel(BaseModel):
	def __init__(self, config):
		super(DetectionModel, self).__init__(config)

		# activate ml run
		if config.MLFLOW.module_name != 'None':
			mlflow.ActiveRun(self.config.mlrun)
			mlflow.keras.autolog()

		# get params from config
		self.weight_path = self.config.model.weight_path
		self.use_pretrained = self.config.model.use_pretrained
		self.pretrained_model_path = self.config.model.pretrained_model_path

		# Input shape
		self.channels = 3
		self.lr_height = self.config.data.image_size_h # Low resolution height
		self.lr_width = self.config.data.image_size_w # Low resolution width
		self.lr_shape = (self.lr_height, self.lr_width, self.channels)

		self.hr_height = self.lr_height // self.config.model.descale_factor # High resolution height
		self.hr_width = self.lr_width // self.config.model.descale_factor # High resolution width
		self.hr_shape = (self.hr_height, self.hr_width, self.channels)

		# Number of residual blocks in the generator
		self.n_residual_blocks = 16

		optimizer = Adam(0.0002, 0.5)

		# We use a pre-trained VGG19 model to extract image features from the high resolution
		# and the genrated high resolution images and minimize the mse between them
		self.vgg = self.build_vgg()
		self.vgg.trainable = False
		self.vgg.compile(loss='mse', optimizer=optimizer, metrics=['accuracy'])

		# Calculate output shape of D (PatchGAN)
		patchx = int(self.hr_width / 2**4)
		patchy = int(self.hr_height / 2**4)

		self.disc_patch = (patchy, patchx, 1)

		# Number of filters in the first layer of G and D
		self.gf = 64
		self.df = 64

		# Build and compile the discriminator
		self.discriminator = self.build_discriminator()
		self.discriminator.compile(loss='mse', optimizer=optimizer, metrics=['accuracy'])

		# Build the generator
		self.generator = self.build_generator()

		# High res. and Low res. images
		img_hr = Input(shape=self.hr_shape)
		img_lr = Input(shape=self.lr_shape)

		# Generate high res. version from low res.
		fake_hr = self.generator(img_lr)

		# Extract image features of the genrated img
		fake_features = self.vgg(fake_hr)

		# For the combined model we will only train the generator
		self.discriminator.trainable = False

		# Discriminator determines validity of generated high res. images
		validity = self.discriminator(fake_hr)

		self.combined = Model([img_lr, img_hr], [validity, fake_features])
		self.combined.compile(loss=['binary_crossentropy', 'mse'], loss_weights=[1e-3, 1], optimizer=optimizer)

		# Load pretrained model
		# 현재 generator만 load 하도록 설계되어 있음 (infernce용)
		# 차후에 continue learning을 위해서는 generator, combined, discriminator 등 모두 load 필요
		if self.config.model.use_pretrained is True:
			print('... load pretrained model')
			if self.config.model.pretrained_model_path[-3:] == '.h5':
				print('... load generator')
				self.generator.load_weights(self.config.model.pretrained_model_path)
			else:
				basemodelpath = self.config.model.pretrained_model_path
				print('...... load generator')
				self.generator.load_weights(basemodelpath+'models_generator/data/model.5')
				print('...... load discriminator')
				self.discriminator.load_weights(basemodelpath+'models_discriminator/data/model.h5')
				print('...... load combined model')
				self.combined.load_weights(basemodelpath+'models_combined/data/model.h5')

	def build_vgg(self):
		"""
		Builds a pre-trained VGG19 model that ouputs image featuers extracted at the third block of the model
		"""

		vgg = VGG19(include_top=False, weights=self.config.model.weight_path, input_shape=self.hr_shape)

		# See outputs to outputs of last conv. layer in block 3
		# See architecture at: https://github.com/keras-team/keras/blob/master/keras/applications/vgg19.py
		vgg.outputs = [vgg.layers[9].output]

		img = Input(shape=self.hr_shape)

		# Extract image features
		# image_features = vgg(img) # tf < 2.2
		vgg_cuts = Model(vgg.input, outputs=vgg.layers[9].output)
		image_features = vgg_cuts(img)

		return Model(img, image_features)

	def build_generator(self):

		def residual_block(layer_input, filters):
			"""Residual block described in paper"""
			d = Conv2D(filters, kernel_size=3, strides=1, padding='same')(layer_input)
			d = Activation('relu')(d)
			d = BatchNormalization(momentum=0.8)(d)
			d = Conv2D(filters, kernel_size=3, strides=1, padding='same')(d)
			d = BatchNormalization(momentum=0.8)(d)
			d = Add()([d, layer_input])
			return d

		def deconv2d(layer_input):
			"""Layers used during upsampling"""
			u = UpSampling2D(size=2)(layer_input)
			u = Conv2D(256, kernel_size=3, strides=1, padding='same')(u)
			u = Activation('relu')(u)
			return u

		def unconv2d(layer_input, strides):
			"""Layers used during downsampling"""
			u = Conv2D(256, kernel_size=3, strides=strides, padding='same')(layer_input)
			u = Activation('relu')(u)
			return u

		# Low resolution image input
		img_lr = Input((None, None, 3))

		# Pre-residual block
		c1 = Conv2D(64, kernel_size=9, strides=1, padding='same')(img_lr)
		c1 = Activation('relu')(c1)

		# Propogate through residual blocks
		r = residual_block(c1, self.gf)
		for _ in range(self.n_residual_blocks - 1):
			r = residual_block(r, self.gf)

		# Post-residual block
		c2 = Conv2D(64, kernel_size=3, strides=1, padding='same')(r)
		c2 = BatchNormalization(momentum=0.8)(c2)
		c2 = Add()([c2, c1])

		# Upsampling
		c2 = unconv2d(c2, self.config.model.descale_factor)

		# Generate high resolution output
		gen_hr = Conv2D(self.channels, kernel_size=9, strides=1, padding='same', activation='tanh')(c2)

		return Model(img_lr, gen_hr)

	def build_discriminator(self):

		def d_block(layer_input, filters, strides=1, bn=True):
			"""Discriminator layer"""
			d = Conv2D(filters, kernel_size=3, strides=strides, padding='same')(layer_input)
			d = LeakyReLU(alpha=0.2)(d)
			if bn:
				d = BatchNormalization(momentum=0.8)(d)
			return d

		# Input img
		d0 = Input(shape=self.hr_shape)

		d1 = d_block(d0, self.df, bn=False)
		d2 = d_block(d1, self.df, strides=2)
		d3 = d_block(d2, self.df*2)
		d4 = d_block(d3, self.df*2, strides=2)
		d5 = d_block(d4, self.df*4)
		d6 = d_block(d5, self.df*4, strides=2)
		d7 = d_block(d6, self.df*8)
		d8 = d_block(d7, self.df*8, strides=2)

		d9 = Dense(self.df*16)(d8)
		d10 = LeakyReLU(alpha=0.2)(d9)
		validity = Dense(1, activation='sigmoid')(d10)

		return Model(d0, validity)

	def detect(self, images):
		boxes_pred = []

		def compute_input(image, expand_dim=True):
			img_array = np.array(image, dtype=np.float32)
			img_array = image_utils.per_image_standardization(img_array)
			if expand_dim:
				img_array = tf.expand_dims(img_array, 0)
			return img_array

		if batch_size > 0:
			tfimage = []
			for idx in tqdm(range(len(images))):
				tfimage.append(compute_input(images[idx], expand_dim=False))
				if (idx + 1) % batch_size == 0:
					pim = np.array(tfimage, dtype=np.float32)
					print(pim.shape)
					box_pred.extend(self.generator.predict(pim))
					tfimage = []
			pim = np.array(tfimage, dtype=np.float32)
			box_pred.extend(self.generator.predict(pim))
		else:
			for idx in tqdm(range(len(images))):
				tfimage = compute_input(images[idx])
				box_pred = self.generator.predict(tfimage)
				boxes_pred.append(box_pred)
		return boxes_pred
