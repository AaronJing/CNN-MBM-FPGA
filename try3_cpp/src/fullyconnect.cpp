#include "fullyconnect.h"


void FULLY_CONNECTED_L1(float* input, float* weight, float* bias, float* output){

	for (int i = 0; i < OUTPUT_NN_1_SIZE; i++){

		float temp=0;
		for (int j = 0; j < INPUT_NN_1_SIZE; j++){

			float in = input[j];
			float w = weight[j*84+i];
			temp += in*w;
		}

		output[i] = tanhf(temp + bias[i]);

	}
}

void FULLY_CONNECTED_L2(float* input, float* weight, float* bias, float* output){
	for (int i = 0; i < OUTPUT_NN_2_SIZE; i++){
		float temp = 0;
		for (int j = 0; j < INPUT_NN_2_SIZE; j++){
			float in = input[j];
			float w = weight[j*10+i];
			temp += in*w;
		}
		output[i] = tanhf(temp+bias[i]);
	}
}
