/*
 * main.c
 *
 *  Created on: 2019Äê3ÔÂ21ÈÕ
 *      Author: biglo
 */


#include <stdio.h>
#include <stdlib.h>
#include "convolution.h"
#include "fullyconnect.h"
#include "pool.h"
#include "malloc.h"
int main (){
	printf("111\n");
//	xil_printf("222\n");
//	print("123\n");

//	float* weights = (float*) malloc (6*sizeof(float));
//	load_data("data.txt",weights, 6 );
	printf("123\n");

	//xil_printf("123");
	float* Wconv1= (float*) malloc(CONV_1_TYPE*CONV_1_SIZE*sizeof(float));
	float* bconv1=(float*)malloc(CONV_1_TYPE*sizeof(float));
	float* Wconv2=(float*)malloc(CONV_2_TYPE*CONV_1_TYPE*CONV_2_SIZE*sizeof(float));
	float* bconv2=(float*)malloc(CONV_2_TYPE*sizeof(float));
	float* Wconv3=(float*)malloc(CONV_3_TYPE*CONV_2_TYPE*CONV_3_SIZE*sizeof(float));
	float* bconv3=(float*)malloc(CONV_3_TYPE*sizeof(float));

	float* Wpool1= (float*) malloc(POOL_1_TYPE*4*sizeof(float));
	float* Wpool2= (float*) malloc(POOL_2_TYPE*4*sizeof(float));
	float* bpool1= (float*) malloc(POOL_1_TYPE*sizeof(float));
	float* bpool2= (float*) malloc(POOL_2_TYPE*sizeof(float));

	float* Wfc1 = (float*) malloc(FILTER_NN_1_SIZE*sizeof(float));
	float* bfc1 = (float*) malloc(BIAS_NN_1_SIZE*sizeof(float));
	float* Wfc2 = (float*) malloc(FILTER_NN_2_SIZE*sizeof(float));
	float* bfc2 = (float*) malloc(BIAS_NN_2_SIZE*sizeof(float));

//	if(!Wconv1||!Wconv2||!Wconv3||!bconv1||!bconv2||!bconv3||!Wpool1||!Wpool2||!bpool1||!bpool2||!Wfc1||!Wfc2||!bfc1||!bfc2){
//		xil_printf("mem alloc error\n");
//		exit(1);
//	}
	float* input_layer	= (float*) malloc(image_Batch *INPUT_WH * INPUT_WH*sizeof(float));
	printf("inputlayer malloc\n");
	float* hconv1 		= (float*) malloc(image_Batch * CONV_1_TYPE * CONV_1_OUTPUT_SIZE*sizeof(float));
	float* pool1 		= (float*) malloc(image_Batch * CONV_1_TYPE * POOL_1_OUTPUT_SIZE*sizeof(float));
	float* hconv2 		= (float*) malloc(image_Batch * CONV_2_TYPE * CONV_2_OUTPUT_SIZE*sizeof(float));
	float* pool2 		= (float*) malloc(image_Batch * CONV_2_TYPE * POOL_2_OUTPUT_SIZE*sizeof(float));
	float* hconv3 		= (float*) malloc(image_Batch * CONV_3_TYPE*sizeof(float));
	float* hfc1 		= (float*) malloc(image_Batch * OUTPUT_NN_1_SIZE*sizeof(float));
	float* output 		= (float*) malloc(image_Batch * OUTPUT_NN_2_SIZE*sizeof(float));
//	if(!input_layer || !hconv1 || !pool1 || !hconv2 || !pool2 || !hconv3 || !hfc1 || !output){
//		xil_printf("mem alloc error2\n");
//		exit(1);
//	}
	printf("inputlayer\n");
	CONV_LAY1(input_layer,Wconv1,bconv1,hconv1);
	printf("inputlayer conv\n");
	POOL_LAYER_1(hconv1,Wpool1,bpool1,pool1);
	CONV_LAY2(pool1,Wconv2,bconv2,hconv2);
	printf("lay2 conv\n");
	POOL_LAYER_2(hconv2,Wpool2,bpool2,pool2);
	CONV_LAY3(pool2,Wconv3,bconv3,hconv3);

	printf("inputlayer conv\n");
	FULLY_CONNECTED_L1(hconv3,Wfc1,bfc1,hfc1);
	printf("fully conv\n");
	FULLY_CONNECTED_L2(hfc1,Wfc2,bfc2,output);

//	xil_printf("asdad");
	printf("123\n");
	free(input_layer);
	free(hconv1);
	free(hconv2);
	free(hconv3);
	free(pool1);
	free(pool2);
	free(hfc1);
	free(output);
	free(Wconv1);
	free(Wconv2);
	free(Wconv3);
	free(bconv1);
	free(bconv2);
	free(bconv3);
	free(Wpool1);
	free(bpool1);
	free(Wpool2);
	free(bpool2);
	free(Wfc1);
	free(bfc1);
	free(Wfc2);
	free(bfc2);
}
