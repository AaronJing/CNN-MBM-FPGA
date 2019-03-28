
#include "cnn.h"
#include <stdio.h>

float input[c1_input_width][c1_input_width]={{0,1,1,1},
				 {1,0,1,1},
				 {1,1,0,1},
				 {1,1,1,0}};

float weight[c1_filter*c1_filter_size*c1_filter_size]={-1,1,1,1,
														1,-1,1,1,
														1,1,-1,1,
														1,1,1,-1,
														-1,1,1,1,
														1,-1,1,1};
float bias[c1_filter]={2,2,2,2,2,2};


int main()
{

	fp_data input_image[c1_input_width][c1_input_width];
	fp_data output[c1_output_width*c1_output_size*c1_output_size];
//	fp_data filter_w_bram[c1_filter*c1_filter_size*c1_filter_size];
//	fp_data filter_b_bram[c1_filter];
//	init_kernel: // 6
//				for(int kernel = 0; kernel < c1_filter; kernel++){
//					init_row:
//						for(int row = 0; row < c1_filter_size; row++){
//							init_col:
//							for(int col = 0; col < c1_filter_size; col++){
//								filter_w_bram[kernel*c1_filter_size*c1_filter_size + row*c1_filter_size + col].data = weight[kernel*c1_filter_size*c1_filter_size + row*c1_filter_size + col];
//								filter_w_bram[kernel*c1_filter_size*c1_filter_size + row*c1_filter_size + col].last = 0;
//							}
//					}
//				}
//				filter_w_bram[c1_filter*c1_filter_size*c1_filter_size-1].last = 1;
//			bias:
//				for(int i = 0; i < c1_filter; i++){
//					filter_b_bram[i].data = bias[i];
//				}
//				filter_b_bram[c1_filter-1].last = 1;

	for(int i = 0; i < c1_input_width; i++){
		for(int j = 0; j < c1_input_width; j++){
			input_image[i][j].data = input[i][j];
			input_image[i][j].last = 0;
		}
	}
	input_image[c1_input_width-1][c1_input_width-1].last = 1;
	c1(input_image,weight,bias,output);
	for(int z = 0; z < c1_output_width;z++){
		printf("%d's feature\n",z);
		for(int i = 0; i < c1_output_size; i++){
			printf("|");
			for(int j = 0; j < c1_output_size; j++){
				printf("%f|",output[z*c1_output_size*c1_output_size+i*c1_output_size+j].data);
			}
			printf("\n");
		}
		printf("\n\n");
	}
	return 0;
}
