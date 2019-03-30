#include "values.h"

#define o true
#define x false






void CONV_LAY1(float* input, float* weight, float* bias, float* output){

	int col, row, col2, row2;
	int depth;
	float temp;
	for (depth = 0; depth < CONV_1_TYPE; depth++){
		for (row = 0; row < CONV_1_OUTPUT_WH; row++){
			for (col = 0; col < CONV_1_OUTPUT_WH; col++){

				temp = 0;

				for (row2 = 0; row2 < CONV_1_WH; row2++){
					for (col2 = 0; col2 < CONV_1_WH; col2++){
						float in = input[INPUT_WH * (row + row2) + col + col2];
						float w = weight[depth * CONV_1_SIZE + CONV_1_WH * row2 + col2];
						temp = temp + in * w;
					}
				}
				output[depth * CONV_1_OUTPUT_WH * CONV_1_OUTPUT_WH + CONV_1_OUTPUT_WH * row + col] = tanhf(temp + bias[depth]);

			}
		}
	}
}

void CONV_LAY2(float* input, float* weight, float* bias, float* output){
	int col, row, col2, row2;
	int depth1, depth2;
	float temp = 0;

	for(int i=0;i<1600;i++){
		output[i] = 0;
	}

	for (depth1 = 0; depth1 < CONV_2_TYPE; depth1++){
		for (depth2 = 0; depth2 < CONV_1_TYPE; depth2++){
			if(!tbl[depth2 * 16 + depth1]) continue;
			for (row = 0; row < CONV_2_OUTPUT_WH; row++){
				for (col = 0; col < CONV_2_OUTPUT_WH; col++){

					temp = 0;

					for (row2 = 0; row2 < CONV_2_WH; row2++){
						for (col2 = 0; col2 < CONV_2_WH; col2++){
							float in = input[depth2 * CONV_2_INPUT_SIZE  + CONV_2_INPUT_WH * (row2 + row) + col + col2];
							float w = weight[(depth1 * CONV_1_TYPE + depth2) * 25 + CONV_2_WH * row2 + col2];
							temp = temp + in * w;
						}
					}
					output[depth1 * CONV_2_OUTPUT_WH * CONV_2_OUTPUT_WH + CONV_2_OUTPUT_WH * row + col] += temp;

				}
			}
		}
		for (int i = 0; i < 100; i++){
			float old = output[depth1*100+i];
			float bia = bias[depth1];
			float newV= old + bia;
			output[depth1*100+i] = newV;
		}
	}
	for (int i = 0; i< 1600; i++){
		output[i] = tanhf(output[i]);
	}
}


void CONV_LAY3(float* input, float* weight, float* bias, float* output){

	int col, row;
	int depth1, depth2;
	float temp = 0;


	for (depth1 = 0; depth1 < CONV_3_TYPE; depth1++){

		temp = 0;

		for (depth2 = 0; depth2 < POOL_2_TYPE; depth2++){
			for (row = 0; row < CONV_3_WH; row++) {
				for (col = 0; col < CONV_3_WH; col++){
					float in = input[depth2 * CONV_3_WH * CONV_3_WH + CONV_3_WH * row + col];
					float w = bias[(depth1 * CONV_2_TYPE + depth2) * 25 + row * 5 + col];
					temp += in * w;
				}
			}
		}
		float bia = bias[depth1];
		output[depth1] = temp + bia;
	}
	for (int i = 0; i < 120; i++){
		output[i] = tanhf(output[i]);
	}


}


