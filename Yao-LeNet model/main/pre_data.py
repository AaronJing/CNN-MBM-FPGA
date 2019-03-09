from tensorflow.examples.tutorials.mnist import input_data
import numpy as np


def pre_data():
	mnist = input_data.read_data_sets("MNIST_data/", reshape = False)
	x_train, y_train = mnist.train.images, mnist.train.labels
	x_validation, y_validation = mnist.validation.images, mnist.validation.labels
	x_test, y_test = mnist.test.images, mnist.test.labels

	assert(len(x_train) == len(y_train))
	assert(len(x_validation) == len(y_validation))
	assert(len(x_test) == len(y_test))


	print("Images Shape: {}".format(x_train[0].shape))
	print("Training Set: {} samples".format(len(x_train)))
	print("Validation Set: {} samples".format(len(x_validation)))
	print("Test Set: {} samples".format(len(x_test)))

	# Pad images with 0s
	x_train = np.pad(x_train, ((0,0), (2,2,), (2,2), (0,0)), 'constant')
	x_validation = np.pad(x_validation, ((0,0), (2,2,), (2,2), (0,0)), 'constant')
	x_test = np.pad(x_test, ((0,0), (2,2,), (2,2), (0,0)), 'constant')

	return x_train, y_train, x_validation, y_validation, x_test, y_test