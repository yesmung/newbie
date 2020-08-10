# -*- coding: utf-8 -*-

from base.base_model import BaseModel

from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Input
from tensorflow.keras.layers import Dense
from tensorflow.keras.layers import Conv2D
from tensorflow.keras.layers import Conv1D
from tensorflow.keras.layers import MaxPooling2D
from tensorflow.keras.layers import Dropout
from tensorflow.keras.layers import Flatten
from tensorflow.keras.layers import Bidirectional
from tensorflow.keras.layers import Embedding
from tensorflow.keras.layers import LSTM
from tensorflow.keras.layers import Attention
from tensorflow.keras.layers import GlobalAveragePooling1D
from tensorflow.keras.layers import Concatenate
from tensorflow.keras.layers import MaxPool1D
from tensorflow.keras.layers import BatchNormalization
from tensorflow.keras.layers import InputLayer
from tensorflow.keras.layers import LeakyReLU

import tensorflow as tf
import tensorflow.keras as keras

import mlflow
import mlflow.tensorflow

class Model(BaseModel):
	def __init__(self, config, tokenizer):
		super(Model, self).__init__(config)

		# activate ml run
		mlflow.ActiveRun(self.config.mlrun)
		mlflow.keras.autolog()

		self.n_cat = config.data.num_categories
		self.word_index_len = len(tokenizer.word_index)+1

		# get params from config
		self.input_size = (config.data.image_size_h, config.data.image_size_w)
		self.weight_path = config.model.weight_path
		self.use_pretrained = config.model.use_pretrained
		self.pretrained_model_path = config.model.pretrained_model_path
		self.build_model()

	def build_model(self):
		# activate ml run
		mlflow.ActiveRun(self.config.mlrun)
		mlflow.tensorflow.autolog(1)

		# get some params
		inputsize = self.input_size

		# define model
		model = Sequential()
		model.add(Embedding(self.word_index_len, 200, trainable=True))

		# conv
		model.add(Conv1D(128, 3, activation=None))
		model.add(LeakyReLU())
		model.add(BatchNormalization())
		model.add(MaxPool1D(2))
		model.add(Conv1D(256, 5, activation=None))
		model.add(LeakyReLU())
		model.add(BatchNormalization())
		model.add(MaxPool1D(2))

		# lstm
		model.add(Bidirectional(LSTM(128, return_sequences=True)))
		model.add(Bidirectional(LSTM(64)))

		model.add(Dense(64, activation=None))
		model.add(LeakyReLU())
		model.add(BatchNormalization())
		model.add(Dropout(0.5))
		model.add(Dense(self.n_cat, activation='softmax'))

		# Load pretrained model
		if self.use_pretrained is True:
			model.load_weights(self.pretrained_model_path)

		# compile model
		self.model = model
		self.model.compile(optimizer=self.config.model.optimizer, loss='categorical_crossentropy', metrics=['accuracy'])