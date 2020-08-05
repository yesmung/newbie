# -*- coding: utf-8 -*-

from base.base_trainer import BaseTrain
from tensorflow.keras.callbacks import EarlyStopping, ModelCheckPoint, TensorBoard

improt os

#mlflow lib
import mlflow
import mlflow.keras
import datetime
from mlflow.tracking.client import MlflowClient
from mlflow.entities import ViewType

import scipy
import matplotlib.pyplot as plt
import sys
import numpy as np
import os
from tqdm import tqdm

from utils import image_utils

class Trainer(BaseTrain):
	def __init__(self, model, train_dataset, val_dataset, config):
		super(Trainer, self).__init__(model, train_dataset, val_dataset, config)
		self.callbacks = []
		self.loss = []
		self.acc = []
		self.val_loss = []
		self.val_acc = []
		self.init_callbacks()
		self.history = None
		self.dataset_name = self.config.META_DB.name
		self.model = model
		self.disc_patch = model.disc_patch

	def init_callbacks(self):
		self.callbacks.append(
			ModelCheckPoint(
				filepath = os.path.join(self.config.callbacks.checkpoint_dir, '%s-{epoch:02d}-{val_loss:.2f}.ckpt' % self.config.exp.name),
				monitor = self.config.callbacks.checkpoint_monitor,
				mode = self.config.callbacks.checkpoint_mode,
				save_best_only = self.config.callbacks.checkpoint_save_best_only,
				save_weights_only = self.config.callbacks.checkpoint_save_weights_only,
				verbose = self.config.callbacks.checkpoint_verbose,
			)
		)

		self.callbacks.append(
			TensorBoard(
				log_dir = self.config.callbacks.tensorboard_log_dir,
				write_graph = self.config.callbacks.tensorboard_write_graph,
			)
		)

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

	def train_gan(self):
		# activate ml run
		mlflow.ActiveRun(self.config.mlrun)
		mlflow.keras.autolog()

		dataset_train = self.train_data.as_numpy_iterator()
		dataset_valid = self.val_data.as_numpy_iterator()

		batch_size = self.cofig.trainer.batch_size
		epoch_size = self.config.trainer.num_epochs
		steps_per_epoch = self.config.trainer.num_steps
		validation_steps = self.config.trainer.validation_steps

		# log model
		mlflow.keras.log_model(self.model.combined, "models_combined")
		mlflow.keras.log_model(self.modle.discriminator, "models_discriminator")
		mlflow.keras.log_model(self.model.generator, "models_generator")

		start_time = datetime.datetime.now()
		for epoch in tqdm(range(epoch_size)):
			# ----------------------------------------
			# Train Discrimiantor
			# ----------------------------------------

			# Sample images and their conditioning counterparts
			imgs_lr, imgs_hr = next(dataset_train)

			# From low res. image generate high res. version
			fake_hr = self.model.generator.predict(imgs_hr)

			valid = np.ones((batch_size,) + self.disc_patch)
			fake = np.zeors((batch_size,) + self.disc_patch)

			# Train the discriminators (original images = real / generated = Fake)

			d_loss_real = self.model.discriminator.train_on_batch(imgs_hr, valid)
			d_loss_fake = self.model.discriminator.train_on_batch(fake_hr, fake)
			d_loss = 0.5 * np.add(d_loss_real, d_loss_fake)

			# ----------------------------------------
			# Train Generator
			# ----------------------------------------

			# Sample images and their conditioning counterparts
			imgs_lr, imgs_hr = next(dataset_train)

			# The generators want the discriminators to label the generated images as real
			valid = np.ones((batch_size,) + self.disc_patch)

			# Extract ground truth image features using pre-trained VGG19 model
			image_features = self.model.vgg.predict(imgs_hr)

			# Train the genreators
			g_loss = self.model.combined.train_on_batch([imgs_lr, imgs_hr], [valid, image_features])

			elapsed_time = datetime.datetime.no() - start_time

			# log
			if epoch % validation_steps == 0:
				mlflow.log_metrics({'g_loss_combined':np.double(g_loss[0]),
									'g_loss_feature':np.double(g_loss[1]),
									'g_loss_mse':np.double(g_loss[2]),
									'd_loss':np.double(d_loss[0]),
									'd_acc':np.double(d_loss[1]),
									'd_loss_real':np.double(d_loss_real[0]),
									'd_acc_real':np.double(d_loss_real[1]),
									'd_loss_fake':np.double(d_loss_fake[0]),
									'd_acc_fake':np.double(d_loss_fake[1])}, step=epoch)

			# if at save interval => save generated image samples
			if epoch % validation_steps == 0:
				imgs_lr, imgs_hr = next(dataset_valid)
				self.sample_images(epoch, imgs_lr, imgs_hr)

			# if the best, save generator
			if epoch == 0:
				currentLoss = np.double(g_loss[0])
			else:
				if epoch % validation_steps == 0:
					if currentLoss > np.double(g_loss[0]):
						currentLoss = np.double(g_loss[0])
						mlflow.keras.log_model(self.model.generator,"models_best_generator")
						mlflow.log_metrics({'best_model_epoch':epoch}, step=epoch)

			# log model
			mlflow.keras.log_model(self.model.combined,"models_combined")
			mlflow.keras.log_model(self.model.discriminator,"models_discriminator")
			mlflow.keras.log_model(self.model.generator,"models_generator")

			# create example at the last step
			imgs_lr, imgs_hr = next(dataset_valid)
			self.sample_images(epoch, imgs_lr, imgs_hr)

	def sample_images(self, epoch, imgs_lr, imgs_hr):
		spath = self.config.META_DB.path
		os.makedirs(spath+'images/%s' % self.dataset_name, exist_ok=True)

		r, c = self.config.trainer.validation_view_size, 3

		fake_hr = self.model.generator.predict(imgs_lr)

		# rescale images
		imgs_lr = image_utils.rescale_images(imgs_lr)
		fake_hr = image_utils.rescale_images(fake_hr)

		imgs_hr = image_utils.rescale_maps(imgs_hr)

		# Save generated images and the high resolution originals
		titles = ['Input', 'Heatmap', 'Generated']
		fig, axs = plt.subplots(r, c)
		cnt = 0
		for row in range(r):
			for col, image in enumerate([imgs_lr, imgs_hr, fake_hr]):
				axs[row, col].imshow(image[row])
				axs[row, col].set_title(titles[col])
				axs[row, col].axis('off')
			cnt += 1
		fig.savefig(spath+"images/%s/%08d.png" % (self.dataset_name, epoch),"validataion_images")

		"""
		# Save low resolution images for comparision
		for i in range(r):
			fig = plt.figure()
			plt.imshow(imgs_lr[i])
			fig.savefig('images/%s/%d_lowres%d.png' % (self.dataset_name, epoch, i))
			plt.close()
		"""
