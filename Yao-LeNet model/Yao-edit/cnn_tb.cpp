
#include "cnn.h"
#include <stdio.h>
#include <math.h>
#include <iostream>
//#define pooling_4_test
//#define conv5_test
//#define output_test
//#define conv3_test
//float input[c1_input_width][c1_input_width]={{0,1,1,1,1},
//				 {1,0,1,1,1},
//				 {1,1,0,1,1},
//				 {1,1,1,0,1},
//				 {1,1,1,1,0}
//};

//float weight[c1_filter*c1_filter_size*c1_filter_size]={-1,1,1,1,
//														1,-1,1,1,
//														1,1,-1,1,
//														1,1,1,-1,
//														-1,1,1,1,
//														1,-1,1,1};
//float bias[c1_filter]={2,2,2,2,2,2};
// max pooling 2 test
//float pooling_input[6][4][4] = {
//		{{1,2,3,4},{3,4,5,6},{5,6,7,8},{7,8,9,10}},
//		{{5,6,7,8},{7,8,9,10},{9,10,11,12},{11,12,13,14}},
//		{{9,10,11,12},{11,12,13,14},{13,14,15,16},{17,18,19,20}},
//		{{13,14,15,16},{17,18,19,20},{21,22,23,24},{22,23,24,25}},
//		{{17,18,19,20},{21,22,23,24},{22,23,24,25},{24,25,26,27}},
//		{{21,22,23,24},{22,23,24,25},{24,25,26,27},{26,27,28,29}},
//};
//float pooling_output[6][2][2];
// max pooling 4 test
#ifdef pooling_4_test
	float pooling_input[16][10][10];
	float pooling_output[16][5][5];
#endif

#ifdef conv5_test
	float conv5_input[16][5][5];
	float weight[120*5*5*16];
	float bias[120];
	float conv5_output[120];
#endif

#ifdef output_test
	float input[120];
	float weight[1200];
	float bias[10];
	float output[10];
#endif

#ifdef conv3_test
	using namespace std;

	float input[6][9];
	float weight[96][4];
	float bias[16];
	float output[16][4];
	int prev_map_count = 6;
	int map_count = 16;

	#define O true
	#define X false

	bool connection_table[6*16] = {
	                O, X, X, X, O, O, O, X, X, O, O, O, O, X, O, O,
	                O, O, X, X, X, O, O, O, X, X, O, O, O, O, X, O,
	                O, O, O, X, X, X, O, O, O, X, X, O, X, O, O, O,
	                X, O, O, O, X, X, O, O, O, O, X, X, O, X, O, O,
	                X, X, O, O, O, X, X, O, O, O, O, X, O, O, X, O,
	                X, X, X, O, O, O, X, X, O, O, O, O, X, O, O, O
	};
	#undef O
	#undef X

	void convn_valid(float *in_data, int in_w, float *kernel, int kernel_w, int kernel_h, float *out_data, int out_w, int out_h) {
	    double sum = 0.0;
	    for (int i = 0; i < out_h; i++) {
	        for (int j = 0; j < out_w; j++) {
	            sum = 0.0;
	            for (int n = 0; n < kernel_h; n++) {
	                for (int m = 0; m < kernel_w; m++) {
	                    sum += in_data[(i + n)*in_w + j + m] * kernel[n*kernel_w + m];
//printf("SW: input %f, weight %f\n",in_data[(i + n)*in_w + j + m],kernel[n*kernel_w + m]);
	                }

	            }

	          // printf("out_Data %f\n",out_data[i*out_w + j]);
	            out_data[i*out_w + j] += sum;
	         //  printf("sw %f\n",out_data[i*out_w + j]);
	          //  cout << "@ " << i*out_w + j << endl;
	          //  cout << sum << endl;
	        }
	    }
	}
#endif
	float input[102400] = {
		#include "input.txt"
	};
	float label[100] = {
		#include "label.txt"
	};

	float c1_w[150]={
		#include "c1w.txt"
	};
	float c1_b[6]={
		#include "c1b.txt"
	};
	float c3_w[2400]={
		#include "c3w.txt"
	};
	float c3_b[16]={
		#include "c3b.txt"
	};
	float c5_w[48000]={
	    #include "c5w.txt"
	};
	float c5_b[120]={
	    #include "c5b.txt"
	};
	float fc_w[1200]={
		#include "ow.txt"
	};
	float fc_b[10]={
		#include "ob.txt"
	};

	fp_data output[10];
	fp_data input_real[32][32];
	float find_max(float* input){
		float max_percentage = input[0];
		float max = 0;
		for(int i = 0; i < 10; i++){
			//printf("%f\n",input[i]);
			if(input[i] > max_percentage){
				max_percentage = input[i];
				max = (float)i;
			}
		}
		return max;
	}
	float out[10];
int main()
{
	float correct = 0;
	for(int z = 0; z < 100; z++){
		for(int i = 0; i < 32; i++){
			for(int j = 0; j < 32; j ++){
				input_real[i][j].data= input[i*32+j+z*1024];
				//input_real[i][j].data= input[i*32+j];
				input_real[i][j].tlast= false;
			}
		}
		input_real[31][31].tlast = true;
		//printf("%d\n",z);
		top(input_real,c1_w,c1_b,c3_w,c3_b,c5_w,c5_b,fc_w,fc_b,output);
		for(int i = 0; i <10; i ++){
			out[i] = output[i].data;
		}
		if(find_max(out) == label[z]){
			correct++;
		}
	}
	printf("The correctness is %f\n",correct/100);
//	fp_data input_image[c1_input_width][c1_input_width];
//	fp_data output[s1_output_width*s1_output_size*s1_output_size];
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

//	for(int i = 0; i < c1_input_width; i++){
//		for(int j = 0; j < c1_input_width; j++){
//			input_image[i][j].data = input[i][j];
//			input_image[i][j].last = 0;
//		}
//	}
//	input_image[c1_input_width-1][c1_input_width-1].last = 1;
//	c1(input_image,weight,bias,output);

	/*maxpooling2(pooling_input,pooling_output);
	for(int z = 0; z < s2_output_width;z++){
		printf("%d's feature\n",z);
		for(int i = 0; i < s2_output_size; i++){
			printf("|");
			for(int j = 0; j < s2_output_size; j++){
				printf("%f|",pooling_output[z][i][j]);
			}
			printf("\n");
		}
		printf("\n\n");
	}*/
#ifdef pooling_4_test
	//populate data
	for(int i = 0; i < 16; i++){
		for(int j = 0; j < 10; j++){
			for(int z = 0; z < 10; z++){
				pooling_input[i][j][z] = i*100 + j*10 + z;
			}
		}
	}
	maxpooling4(pooling_input,pooling_output);
	for(int z = 0; z < 16;z++){
		printf("%d's feature\n",z);
		for(int i = 0; i < 5; i++){
			printf("|");
			for(int j = 0; j < 5; j++){
				printf("%f|",pooling_output[z][i][j]);
			}
			printf("\n");
		}
		printf("\n\n");
	}
#endif

#ifdef conv5_test
	//populate data
	for(int i = 0; i < 16; i++){
			for(int j = 0; j < 5; j++){
				for(int z = 0; z < 5; z++){
					conv5_input[i][j][z] = i*25 + j*5 + z;
			}
		}
	}
	int c = 0;
	for(int i = 0; i < 120; i++){
		for(int j = 0; j < 16; j++){
			for(int z = 0; z < 5; z++){
				for(int k = 0; k < 5;k++){
					weight[i*16*5*5+j*5*5+z*5+k]= c;
				}
			}
		}
		c++;
		bias[i] = 1;
	}
	conv5(conv5_input,weight,bias,conv5_output);
	for(int i = 0; i < 120; i++){
		printf("%f\n",conv5_output[i]);
	}
#endif

#ifdef output_test
	//populate data
	for(int i =0; i < 120;i++){
		for(int j = 0; j < 10; j++){
			weight[i*10+j] = i*10+j;
		}
		input[i] = 0.5;
	}
	for(int j = 0; j < 10; j++){
		bias[j] = 1.4;
	}
	output6(input,weight,bias,output);
	for(int i = 0; i < 10; i++){
		printf("%f\n",output[i]);
	}
#endif

#ifdef conv3_test
	float counter = 0.000;
	for (int i = 0; i<6; i++){
		for (int j = 0; j < 9; j++){
			input[i][j] = counter;
			counter+=0.001;
		}
	}

	for (int i = 0; i<96; i++){
		for (int j = 0; j < 4; j++){
			weight[i][j] = counter;
			counter+=0.001;
		}
	}

	for (int i = 0; i < 16; i++){
		bias[i] = counter;
		counter+=0.001;
	}

	// ------------ finishing initializing the input, weight & bias --------------------
	// ----------------------- testing -------------------
	int index = 0;
	int size = 4;
	for (int i = 0; i < map_count; i++) {   // for each 16 output
		for (int j = 0; j < prev_map_count; j++) { // roll over 6 input
			index = j*map_count + i;
			if (!connection_table[index]) {
				continue;
			}
			convn_valid( // check the logic, TODO
					input[j], 3,
					weight[index], 2, 2,
					output[i], 2, 2); // output w/h = 3 - 2 + 1 = 2 by 2
		}

		for (int k = 0; k < size; k++) {
			//printf("SW: %f, %f\n",output[i][k],bias[i]);
			output[i][k] = tanh(output[i][k] + bias[i]);

		}
	}


	cout << endl;
	// display the result
	cout << "SW result:" << endl;
	for (int i = 0; i < map_count; i++){
		cout << "output feature map " << i << endl;
		for (int j = 0; j < 2; j++){
			for (int k = 0; k < 2; k++){
				cout << output[i][j*2+k] << " ";
			}
			cout << endl;
		}
		cout << endl;
	}
	float input_t[6][3][3];
	float weight_t[6*2*2*16];
	float bias_t[16];
	float output_t[16][4];
	//populate data
	counter = 0.000;
	for (int i = 0; i<6; i++){
		for (int j = 0; j < 3; j++){
			for(int z = 0; z < 3; z++){
				input_t[i][j][z] = counter;
				counter+=0.001;
			}
		}
	}
	for (int i = 0; i<96*4; i++){
			weight_t[i] = counter;
			counter+=0.001;
	}
	for (int i = 0; i < 16; i++){
			bias_t[i] = counter;
			counter+=0.001;
	}
	for(int j = 0; j < 16;j++){
		for(int i = 0; i < 4; i++){
			output_t[j][i] = 0;
		}
	}
	conv3(	input_t,
				weight_t,
				bias_t,
				output_t);
	cout << endl;
		// display the result
	cout << "HW result:" << endl;
	for (int i = 0; i < map_count; i++){
		cout << "output feature t map " << i << endl;
		for (int j = 0; j < 2; j++){
			for (int k = 0; k < 2; k++){
				cout << tanh(output_t[i][j*2+k]) << " ";
			}
			cout << endl;
		}
		cout << endl;
	}
#endif
	return 0;
}
