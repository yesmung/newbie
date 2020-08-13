# -*- coding: utf-8 -*-

from base.base_trainer import BaseTrain
from tensorflow.keras.callbacks import EarlyStopping, ModelCheckpoint, TensorBoard

import os

#mlflow lib
import mlflow
import mlflow.tensorflow
from mlflow.tracking.client import MlflowClient
from mlflow.entities import ViewType

import numpy as np

class Trainer(BaseTrain):
	def __init__(self, model, dataset, config):
		# super(Trainer, self).__init__(model, train_dataset, val_dataset, config)
		self.model = model
		self.data =  dataset
		self.config = config

		self.callbacks = []
		self.loss = []
		self.acc = []
		self.val_loss = []
		self.val_acc = []

	def init_callbacks(self):
		self.callbacks.append(
			ModelCheckpoint(
				filepath = os.path.join(self.config.callbacks.checkpoint_dir, '%s-{epoch:02d}-{val_loss:.2f}.ckpt' % self.config.exp.name),
				monitor = self.config.callbacks.checkpoint_monitor,
				mode = self.config.callbacks.checkpoint_mode,
				save_best_only = self.config.callbacks.checkpoint_save_best_only,
				save_weights_only = self.config.callbacks.checkpoint_save_weights_only,
				verbose = self.config.callbacks.checkpoint_verbose,
			)
		)

		# self.callbacks.append(
		# 	TensorBoard(
		# 		log_dir = self.config.callbacks.tensorboard_log_dir,
		# 		write_graph = self.config.callbacks.tensorboard_write_graph,
		# 	)
		# )

	def train(self):
		# activate ml run
		mlflow.ActiveRun(self.config.mlrun)
		mlflow.keras.autolog()

		dataset_train = self.train_data
		dataset_valid = self.val_data
		epoch_size = self.config.trainer.num_epochs
		steps_per_epoch = self.config.trainer.num_steps
		validation_steps = self.config.trainer.validation_steps

		history = self.model.fit(dataset_train, epochs=epoch_size, steps_per_epoch=steps_per_epoch, workers=1)

		self.loss.extend(history.history['loss'])
		self.acc.extend(history.history['accuracy'])

		self.history = history

	def train(self):
		# activate ml run
		mlflow.ActiveRun(self.config.mlrun)
		mlflow.tensorflow.autolog(1)

		dataset_train = self.data[0]
		dataset_valid = self.data[1]
		epoch_size = self.config.trainer.num_epochs
		steps_per_epoch = self.config.trainer.num_steps

		self.callbacks = []

		# if no path is given, use the meta db path
		if self.config.callbacks.checkpoint_dir == "":
			self.config.callbacks.checkpoint_dir = self.config.META_DB.path + self.config.META_DB.name + '/base_model/'

		os.makedirs(self.config.callbacks.checkpoint_dir, exist_ok=True)
		self.callbacks.append(
			ModelCheckpoint(
				filepath = self.config.callbacks.checkpoint_dir+'model.h5',
				monitor = self.config.callbacks.checkpoint_monitor,
				mode = self.config.callbacks.checkpoint_mode,
				save_best_only = self.config.callbacks.checkpoint_save_best_only,
				save_weights_only = self.config.callbacks.checkpoint_save_weights_only,
				verbose = self.config.callbacks.checkpoint_verbose,
			)
		)
		history = self.model.fit(dataset_train, 
			epochs=epoch_size,
			steps_per_epoch=steps_per_epoch,
			use_multiprocessing=False,
			validation_data=dataset_valid,
			verbose=self.config.trainer.verbose_training,
			callbacks=self.callbacks)

		mlflow.log_artifacts(self.config.callbacks.checkpoint_dir, 'best_model')

		self.loss.extend(history.history['loss'])
		self.acc.extend(history.history['accuracy'])
		self.val_loss.extend(history.history['val_loss'])
		self.val_acc.extend(history.history['val_accuracy'])

		self.history = history

	def inference(self):
		predictions = self.model.predict(self.data, verbose=1)
		return predictions
