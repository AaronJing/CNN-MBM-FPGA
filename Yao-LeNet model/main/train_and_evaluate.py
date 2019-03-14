from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

from sklearn.utils import shuffle
import pre_data
from pre_data import pre_data
import tensorflow as tf 

import tensorflow as tf
from tensorflow.contrib.layers import flatten

import numpy

import sys

x_train,y_train,x_validation,y_validation,x_test,y_test = pre_data()
x_train, y_train = shuffle(x_train, y_train)
EPOCHS = 10
BATCH_SIZE = 128

x = tf.placeholder(tf.float32, (None, 32,32,1))
y = tf.placeholder(tf.int32, (None))
one_hot_y = tf.one_hot(y, 10)

rate = 0.001

# LeNet-5 Model

def init_weight(shape):
	w = tf.truncated_normal(shape=shape, mean = 0, stddev = 0.1)
	return tf.Variable(w)


def init_bias(shape):
	b = tf.zeros(shape)
	return tf.Variable(b)

# name:      conv1: C1
# structure: Input = 32x32x1. Output = 28x28x6.
# weights:   (5*5*1+1)*6
# connections: (28*28*5*5+28*28)*

conv1_w = init_weight((5,5,1,6))
conv1_b = init_bias(6)
conv1 = tf.nn.conv2d(x, conv1_w, strides = [1,1,1,1], padding = 'VALID') + conv1_b
conv1 = tf.nn.relu(conv1)

# polling layer : S2
# input: 28x28x6 -> output: 14x14x6
conv1 = tf.nn.max_pool(conv1, ksize = [1,2,2,1], strides = [1,2,2,1], padding= 'VALID')

#  C3
#input 14x14x6 Output = 10x10x16.
#weights: (5*5*6+1)*16 ---real Lenet-5 is (5*5*3+1)*6+(5*5*4+1)*9+(5*5*6+1)*1
conv2_w = init_weight((5,5,6,16))
conv2_b = init_bias(16)
conv2 = tf.nn.conv2d(conv1, conv2_w, strides = [1,1,1,1], padding='VALID') + conv2_b
conv2 = tf.nn.relu(conv2)

# polling layer: S4
# input: 10x10x16 -> output: 5x5x16
conv2 = tf.nn.max_pool(conv2, ksize = [1,2,2,1], strides=[1,2,2,1], padding='VALID')

# into fully connected layer: F5
# input: 5x5x16 -> output: 400
fc0 = flatten(conv2)

# input: 400 -> output:120
fc1_w = init_weight((400,120))
fc1_b = init_bias(120)
fc1 = tf.matmul(fc0, fc1_w) + fc1_b

# input = 120, output = 84
fc2_w = init_weight((120,84))
fc2_b = init_bias(84)
fc2 = tf.matmul(fc1, fc2_w) + fc2_b
fc2 = tf.nn.relu(fc2)


#input: 84, output: 10
fc3_w = init_weight((84,10))
fc3_b = init_bias(10)
logits = tf.matmul(fc2, fc3_w) + fc3_b


# LeNet-5 model

cross_entropy = tf.nn.softmax_cross_entropy_with_logits(logits = logits, labels = one_hot_y)
loss_operation = tf.reduce_mean(cross_entropy)
optimizer = tf.train.AdamOptimizer(learning_rate = rate)

training_operation = optimizer.minimize(loss_operation)

correct_prediction = tf.equal(tf.argmax(logits, 1), tf.argmax(one_hot_y, 1))
accuracy_operation = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
saver = tf.train.Saver()

def evaluate(x_data, y_data):
	num_examples = len(x_data)
	total_accuracy = 0
	sess = tf.get_default_session()
	for offset in range(0, num_examples, BATCH_SIZE):
		batch_x, batch_y = x_data[offset:offset+BATCH_SIZE], y_data[offset:offset+BATCH_SIZE]
		accuracy = sess.run(accuracy_operation, feed_dict={x: batch_x, y:batch_y})
		total_accuracy += (accuracy*len(batch_x))

	return total_accuracy/num_examples

with tf.Session() as sess:
	sess.run(tf.global_variables_initializer())
	num_examples = len(x_train)

	print("Training ...")
	print()

	for i in range(EPOCHS):
		x_train, y_train = shuffle(x_train, y_train)
		for offset in range(0, num_examples, BATCH_SIZE):
			end = offset + BATCH_SIZE
			batch_x, batch_y = x_train[offset:end], y_train[offset:end]
			sess.run(training_operation, feed_dict={x:batch_x, y:batch_y})

		validation_accuracy = evaluate(x_validation, y_validation)
		print("EPOCH {} ...".format(i+1))
		print("Validation Accuracy = {:.3f}".format(validation_accuracy))
		print()

	saver.save(sess, 'lenet')
	print("Model saved")

	# output conv1

	weights_ = conv1_w.eval()
	biases_ = conv1_b.eval()
	weights_ = weights_.reshape(1,-1)
	biases_ = biases_.reshape(1,-1)
	with open('conv1_w.txt', 'w') as conv1_wf:
		numpy.savetxt(conv1_wf, weights_)
	conv1_wf.close()
	with open('conv1_b.txt', 'w') as conv1_bf:
		numpy.savetxt(conv1_bf, biases_)
	conv1_bf.close()

	# output conv2

	weights_ = conv2_w.eval()
	biases_ = conv2_b.eval()
	weights_ = weights_.reshape(1,-1)
	biases_ = biases_.reshape(1,-1)
	with open('conv2_w.txt', 'w') as conv2_wf:
		numpy.savetxt(conv2_wf, weights_)
	conv2_wf.close()
	with open('conv2_b.txt', 'w') as conv2_bf:
		numpy.savetxt(conv2_bf, biases_)
	conv2_bf.close()

	# output fc1

	weights_ = fc1_w.eval()
	biases_ = fc1_b.eval()
	weights_ = weights_.reshape(1,-1)
	biases_ = biases_.reshape(1,-1)
	with open('fc1_w.txt', 'w') as fc1_wf:
		numpy.savetxt(fc1_wf, weights_)
	fc1_wf.close()
	with open('fc1_b.txt', 'w') as fc1_bf:
		numpy.savetxt(fc1_bf, biases_)
	fc1_bf.close()

	# output fc2

	weights_ = fc2_w.eval()
	biases_ = fc2_b.eval()
	weights_ = weights_.reshape(1,-1)
	biases_ = biases_.reshape(1,-1)
	with open('fc2_w.txt', 'w') as fc2_wf:
		numpy.savetxt(fc2_wf, weights_)
	fc2_wf.close()
	with open('fc2_b.txt', 'w') as fc2_bf:
		numpy.savetxt(fc2_bf, biases_)
	fc2_bf.close()

	# output fc3

	weights_ = fc3_w.eval()
	biases_ = fc3_b.eval()
	weights_ = weights_.reshape(1,-1)
	biases_ = biases_.reshape(1,-1)
	with open('fc3_w.txt', 'w') as fc3_wf:
		numpy.savetxt(fc3_wf, weights_)
	fc3_wf.close()
	with open('fc3_b.txt', 'w') as fc3_bf:
		numpy.savetxt(fc3_bf, biases_)
	fc3_bf.close()


with tf.Session() as sess:
	saver.restore(sess, tf.train.latest_checkpoint('.'))
	test_accuracy = evaluate(x_test, y_test)
	print("Test Accuracy = {:.3f}".format(test_accuracy))

