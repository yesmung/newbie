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
from detection_D2GAN.utils import tools
from detection_D2GAN.utils import image_utils
from detection_D2GAN.utils.image_utils import drawBoxes

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
		boxes_char = []
		boxes_word = []
		boxes_pred = []

		def compute_input(image):
			img_array = np.array(image, dtype=np.float32)
			img_array = image_utils.per_image_standardization(img_array)
			img_array = tf.expand_dims(img_array, 0)
			return img_array

		for idx in tqdm(range(len(images))):
			tfimage = compute_input(images[idx])

			box_pred = self.generator.predict(tfimage)

			boxes_pred.append(box_pred)
			boxes_char.append(image_utils.getTextBoxes(bxo_pred,
				text_threshold=self.config.detect.char_text_threshold,
				descale = self.config.model.descale_factor)[0])
			boxes_word.append(image_utils.getBoxes(box_pred,
				detection_threshold = self.config.detect.word_detection_threshold,
				link_threshold=self.config.detect.word_link_threshold,
				text_threshold=self.config.detect.word_text_threshold,
				size_threshold=self.config.detect.word_size_threshold,
				descale=self.config.model.descale_factor)[0])

		return boxes_char, boxes_word, boxes_pred

	def inference_and_save_to_db(self, data_loader, save_image=True, use_maindb=True):

		print('...load data')
		images = data_loader.get_inference_data_online(use_maindb)

		datalen = data_loader.datalen

		# save to database
		config = self.config
		db_meta = data_loader.db_meta

		# get label reference data
		refs = read_all_data_from_meta_db(db_meta, 'ref')
		labels = read_all_data_from_meta_db(db_meta, 'label')

		path = config.inference.data_db_path
		name = config.inference.data_db_name

		# DB load
		if config.inference.create_new_data_db == True:
			# create empty db
			description = config.inference.data_db_description
			dtadb = create_data_db(path=path, name=name, description=description, data_class='INFERENCE')
		else:
			datadb = open_env(path+name)

		# for preview
		name = config.inference.data_db_name + config.inference.preview_db_postfix
		description = 'PREVIEW' + config.inference.data_db_description
		previewdb = create_data_db(path=path, name=name, description=description, data_class='PREVIEW')

		pbar = tqdm(total=datalen)

		imgidx = 0
		for image in images:
			img_array = image_utils.per_image_standardization(image)

			box_pred = self.generator.predict(img_array)
			boxes_char = image_utils.getTextBoxes(box_pred,
				text_threshold=self.config.detect.char_text_threshold,
				dscale=self.config.model.descale_factor)[0]
			boxes_word = image_utils.getBoxes(box_pred,
				detection_threshold=self.config.detect.word_detection_threshold,
				link_threshold=self.config.detect.word_link_threshold,
				text_threshold=self.config.detect.word_text_threshold,
				size_threshold=self.config.detect.word_size_threshold,
				dscale=self.config.model.descale_factor)[0]

			theimage_pil = Image.fromarray(np.array(image, dtype=np.uint8)[0,])
			roi_char = []
			table_char = pd.DataFrame({'x1':[], 'y1':[], 'x2':[], 'y2':[], 'x3':[], 'y3':[], 'x4':[], 'y4':[], 'char':[], 'index':[]})

			rx, ry = 2, 2
			for idx in range(len(boxes_char)):
				# save image
				thebox = boxes_char[idx]
				try:
					if save_image is True:
						roi_char.append(theimage_pil.crop((thebox[0][0]-rx, thebox[0][1]-ry, thebox[2][0]+rx, thebox[2][1]+ry)))
					# save box coordinate
					left, top, right, bottom = thebox[0][0], thebox[0][1], thebox[2][0], thebox[2][1]

					char_data = pd.DataFrame({'x1':[left], 'y1':[top],
						'x2':[right], 'y2':[top],
						'x3':[right], 'y3':[bottom],
						'x4':[left], 'y4':[bottom],
						'char':['N.I.'], 'index':[idx]
						})
					table_char = table_char.append(char_data, ignore_index=True)
				except:
					pass

			# make word list
			roi_word = []
			rx, ry = 2, 2

			table_word = pd.DataFrame({'x1':[], 'y1':[], 'x2':[], 'y2':[], 'word':[], 'index':[]})
			for idx in range(len(boxes_word)):
				# save image
				thebox = boxes_word[idx]
				try:
					if save_image is True:
						roi_word.append(theimage_pil.crop((thebox[0][0]-rx, thebox[0][1]-ry, thebox[2][0]+rx, thebox[2][1]+ry)))
						# save box coordinate
						save_box = [thebox[0][0], thebox[0][1], thebox[2][0], thebox[2][1]]
						wcell = pd.DataFrame({'x1':[save_box[0]], 'y1':[save_box[1]], 'x2':[save_box[2]], 'y3':[save_box[3]], 'word':['N.I.'], 'index':[int(idx)]})
						table_word = table_word.append(wcell, ignore_index=True)
				except:
					pass

			# update to db
			if save_image is True:
				update_data(datadb, index=imgidx, label='detection_d2gan',
					image=theimage_pil, image_c=roi_char, image_w=roi_word,
					char_c=table_char, word_c=table_word, ref=None)
			else:
				update_data(datadb, index=imgidx, label='detection_d2gan',
					char_c=table_char, word_c=table_word, ref=None)

			# make preview
			# draw boxed image
			theimage_boxed_array = drawBoxes(np.array(theimage_pil), boxes_word, thickness=2, color=(0,255,0))
			theimage_boxed_array_char = drawBoxes(theimage_boxed_array, boxes_char, thickness=1, color=(255,0,0))
			theimage_boxed_char = Image.fromarray(theimage_boxed_array_char)

			# update to db
			update_data(previewdb, index=imgidx, label='detection_d2gan_preview', image=theimage_boxed_char, ref=None)

			imgidx = imgidx + 1
			pbar.update(1)

		# DATA DB 시간 업데이트
		update_time_stemp(datadb)
		print_env(dataedb)

		update_time_stemp(previewdb)
		print_env(previewdb)

		# MAIN META DB에 DATA DB 정보 반영(REGISTER)
		db_main = register_db(env=db_meta, db=datadb)
		db_main = register_db(env=db_meta, db=previewdb)

		return datadb