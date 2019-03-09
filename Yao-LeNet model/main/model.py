import tensorflow as tf
from tensorflow.contrib.layers import flatten

def init_weight(shape):
	w = tf.truncated_normal(shape=shape, mean = 0, stddev = 0.1)
	return tf.Variable(w)


def init_bias(shape):
	b = tf.zeros(shape)
	return tf.Variable(b)

def LeNet(x):
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

	return logits


