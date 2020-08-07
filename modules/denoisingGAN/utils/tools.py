# -*- coding: utf-8 -*-

import cv2

def read(filepath):
	""" Read an image from disk.

	Args:
		filepath: The path to the image

	Returns:
		The new image
	"""
	image = cv2.imread(filepath)
	if image is None:
		raise Exception(f'Could not read {filepath}')
	image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
	return image