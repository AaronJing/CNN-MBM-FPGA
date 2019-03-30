/*
 * pool.cpp
 *
 *  Created on: 2019Äê3ÔÂ20ÈÕ
 *      Author: biglo
 */

#include "pool.h"

void POOL_LAYER_1(float* input, float* kernel, float* bias, float* output){
	int row, col, depth;
	int scale = 2;
	float temp;
	for (depth = 0; depth < POOL_1_TYPE; depth++){
			for (row = 0; row < POOL_1_OUTPUT_WH; row++){
				for (col = 0; col < POOL_1_OUTPUT_WH; col++){
					temp = output[depth*POOL_1_INPUT_SIZE + 2*row*POOL_1_INPUT_WH + (col*2)]
					     + output[depth*POOL_1_INPUT_SIZE + 2*row*POOL_1_INPUT_WH + (col*2 +1)]
					     + output[depth*POOL_1_INPUT_SIZE + (2*row + 1)*POOL_1_INPUT_WH + (col*2)]
					     + output[depth*POOL_1_INPUT_SIZE + (2*row + 1)*POOL_1_INPUT_WH + (col*2 + 1)];
					float weight = kernel[depth] * 0.25;
					temp = temp * weight;
					temp += bias[depth];

					output[depth*POOL_1_OUTPUT_SIZE + row * POOL_1_OUTPUT_WH + col] =tanhf(temp);
				}
			}
	}
}

void POOL_LAYER_2(float* input, float* kernel, float* bias, float* output){
	int row, col,  depth;
	int scale = 2;
	float temp;
	for (depth = 0; depth < POOL_2_TYPE; depth++){
			for (row = 0; row < POOL_2_OUTPUT_WH; row++){
				for (col = 0; col < POOL_2_OUTPUT_WH; col++){
					temp = output[depth*POOL_2_INPUT_SIZE + 2*row*POOL_2_INPUT_WH + (col*2)]
					     + output[depth*POOL_2_INPUT_SIZE + 2*row*POOL_2_INPUT_WH + (col*2 +1)]
					     + output[depth*POOL_2_INPUT_SIZE + (2*row + 1)*POOL_2_INPUT_WH + (col*2)]
					     + output[depth*POOL_2_INPUT_SIZE + (2*row + 1)*POOL_2_INPUT_WH + (col*2 + 1)];
					float weight = kernel[depth] * 0.25;
					temp = temp * weight;

					output[depth*POOL_2_OUTPUT_SIZE + row * POOL_2_OUTPUT_WH + col] =tanhf(temp + bias[depth]);
				}
			}

	}
}
