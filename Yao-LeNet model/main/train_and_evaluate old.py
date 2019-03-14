from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

from sklearn.utils import shuffle
import pre_data
from pre_data import pre_data
import tensorflow as tf 

import model
from model import LeNet

x_train,y_train,x_validation,y_validation,x_test,y_test = pre_data()
x_train, y_train = shuffle(x_train, y_train)
EPOCHS = 10
BATCH_SIZE = 128

x = tf.placeholder(tf.float32, (None, 32,32,1))
y = tf.placeholder(tf.int32, (None))
one_hot_y = tf.one_hot(y, 10)

rate = 0.001

logits = LeNet(x)
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

with tf.Session() as sess:
	saver.restore(sess, tf.train.latest_checkpoint('.'))
	test_accuracy = evaluate(x_test, y_test)
	print("Test Accuracy = {:.3f}".format(test_accuracy))

