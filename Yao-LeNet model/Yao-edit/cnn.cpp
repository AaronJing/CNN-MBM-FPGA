

#include "cnn.h"
#include <math.h>
//#include <stdio.h> // only for simulation, remeber there is no file system in HLS




/*float _tanh( const float x )
{
#pragma HLS INLINE
	const float ax = fabsf( x );
	const float x2 = x * x;
	const float z = x * ( 0.773062670268356 + ax +
		( 0.757118539838817 + 0.0139332362248817 * x2 * x2 ) *
		x2 * ax );

	return( z / ( 0.795956503022967 + fabs( z )));
}*/

float _tanh(float x){
	float exp2x =  expf(2*x)+1;
	return (exp2x-2)/(exp2x);
//	return atanf(x);
}



void top(
		fp_data input_image[c1_input_size][c1_input_size],
		float filter_w_c1[c1_filter*c1_filter_size*c1_filter_size],
		float filter_b_c1[c1_filter],
		float filter_w_c3[c3_filter_width*c3_filter_size*c3_filter_size*c3_filter],
		float filter_b_c3[c3_filter],
		float filter_w_c5[fc_output_width*s4_output_size*s4_output_size*c3_output_width],
		float filter_b_c5[fc_output_width],
		float filter_w_fc[fc_output_width*output_width],
		float filter_b_fc[output_width],
		fp_data output[output_size*output_width]

		){

	#pragma HLS INTERFACE axis port=output
	#pragma HLS INTERFACE axis port=input_image
	#pragma HLS INTERFACE s_axilite port=return bundle=CRTL_BUS
	#pragma HLS INTERFACE s_axilite port=filter_w_c1 bundle=KERNEL_BUS
	#pragma HLS INTERFACE s_axilite port=filter_b_c1 bundle=KERNEL_BUS
	#pragma HLS INTERFACE s_axilite port=filter_w_c3 bundle=KERNEL_BUS
	#pragma HLS INTERFACE s_axilite port=filter_b_c3 bundle=KERNEL_BUS
	#pragma HLS INTERFACE s_axilite port=filter_w_c5 bundle=KERNEL_BUS
	#pragma HLS INTERFACE s_axilite port=filter_b_c5 bundle=KERNEL_BUS
	#pragma HLS INTERFACE s_axilite port=filter_w_fc bundle=KERNEL_BUS
	#pragma HLS INTERFACE s_axilite port=filter_b_fc bundle=KERNEL_BUS

	float input_image_bram[c1_input_size][c1_input_size];
#pragma HLS RESOURCE variable=input_image_bram core=RAM_2P_BRAM
	float conv1_out[c1_filter][c1_output_size][c1_output_size];
#pragma HLS RESOURCE variable=conv1_out core=RAM_2P_BRAM
	float maxp2_out[s2_output_width][s2_output_size][s2_output_size];
#pragma HLS RESOURCE variable=maxp2_out core=RAM_2P_BRAM

	float conv3_out[c3_output_width][c3_output_size][c3_output_size];
#pragma HLS RESOURCE variable=conv3_out core=RAM_2P_BRAM
	float maxp4_out[c3_output_width][s4_output_size][s4_output_size];
#pragma HLS RESOURCE variable=maxp4_out core=RAM_2P_BRAM
	float conv5_out[fc_output_width];
#pragma HLS RESOURCE variable=conv5_out core=RAM_2P_BRAM
	float final_layer_out[output_width];
#pragma HLS RESOURCE variable=final_layer_out core=RAM_2P_BRAM

//#pragma HLS RESOURCE variable=cnn_output core=RAM_1P_BRAM
//	float filter_w_bram[c1_filter*c1_filter_size*c1_filter_size];
//#pragma HLS RESOURCE variable=filter_w_bram core=RAM_2P_BRAM
//	float filter_b_bram[c1_filter];
//#pragma HLS RESOURCE variable=filter_b_bram core=RAM_2P_BRAM

//	if(init){
//		init_kernel: // 6
//			for(int kernel = 0; kernel < c1_filter; kernel++){
//				init_row:
//					for(int row = 0; row < c1_filter_size; row++){
//						init_col:
//						for(int col = 0; col < c1_filter_size; col++){
//							filter_w_bram[kernel*6+row*2+col] = filter_w[kernel*c1_filter_size*c1_filter_size + row*c1_filter_size + col].data;
//					}
//				}
//			}
//		bias:
//			for(int i = 0; i < c1_filter; i++){
//				filter_b_bram[i] = filter_b[i].data;
//			}
//	}

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ c1
	image:
	init_image_col:
	for(int row = 0; row < 32; row++){
		init_image_row:
		for(int col = 0; col < 32; col++){
#pragma HLS PIPELINE II=1
			input_image_bram[row][col] = input_image[row][col].data;
		}
	}


	// init c1_w

	float c1_w[c1_filter][c1_filter_size][c1_filter_size];
	float c1_b[c1_filter];
	float out_c1[c1_filter][c1_output_size][c1_output_size];

#pragma HLS array_partition variable=c1_w complete dim=1
#pragma HLS array_partition variable=c1_b complete dim=0
#pragma HLS array_partition variable=out_c1 complete dim=1 /////////dim = 2???


	c1_w:
	init_c1_w:
	for(int num_filter; num_filter < c1_filter; num_filter++){
		for(int row_k = 0; row_k < c1_filter_size; row_k++){
			for(int col_k = 0; col_k < c1_filter_size; col_k++){
#pragma HLS PIPELINE II=1
				c1_w[num_filter][row_k][col_k] = filter_w_c1[num_filter*25+row_k*5+col_k];
			}
		}
	}


	copy_bias_C1:
	for(int i = 0; i < 6; i++){
#pragma HLS PIPELINE II=1
		c1_b[i] = filter_b_c1[i];
	}

	BATCH:
	for(int row_k=0; row_k < c1_filter_size; row_k++){
		for(int col_k=0; col_k < c1_filter_size; col_k++){
			for(int row=0; row<c1_output_size; row++){
				for(int col=0; col<c1_output_size; col++){
#pragma HLS PIPELINE II=1
					float input_pixel = input_image_bram[row+row_k][col+col_k];
					float mult[6];
#pragma HLS array_partition variable=mult complete dim=0
					D_OUT:
					for(int co = 0; co < 6; co++){
#pragma HLS unroll
						mult[co] = input_pixel*c1_w[co][row_k][col_k];
						if(row_k==0 && col_k == 0){
							out_c1[co][row][col] = mult[co];
						}else{
							out_c1[co][row][col] += mult[co];
						}

					}

				}
			}
		}
	}


	for(int i = 0; i < c1_filter; i++){
		for(int row = 0; row < c1_output_size; row++){
			for(int col=0; col < c1_output_size; col++){
				out_c1[i][row][col] = _tanh(out_c1[i][row][col]+c1_b[i]);

			}
		}
	}

	//conv1(input_image_bram,filter_w_c1,filter_b_c1,conv1_out);

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	maxpooling2(out_c1,maxp2_out);

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ c3
	//conv3(maxp2_out,filter_w_c3,filter_b_c3,conv3_out);

	static bool connection_table[6*16] = {
	                true, false, false, false, true, true, true, false, false, true, true, true, true, false, true, true,
	                true, true, false, false, false, true, true, true, false, false, true, true, true, true, false, true,
	                true, true, true, false, false, false, true, true,true, false, false, true, false, true, true,true,
	                false, true, true, true, false, false, true, true, true, true, false, false, true, false, true, true,
	                false, false, true, true, true, false, false, true, true, true, true, false, true, true, false, true,
	                false, false, false, true, true, true, false, false, true, true, true, true, false, true, true, true
	};

	static const int C2_N_PE = 2;

	float WBRAM[c3_filter][c3_filter_width][c3_filter_size*c3_filter_size];
	float biasBRAM[c3_filter];
	float OBRAM[c3_output_width][c3_output_size*c3_output_size];

#pragma HLS array_partition variable=WBRAM complete dim=2
#pragma HLS array_partition variable=WBRAM cyclic factor=C2_N_PE dim=1
#pragma HLS array_partition variable=biasBRAM complete dim=0
#pragma HLS array_partition variable=OBRAM cyclic factor=C2_N_PE dim=2

	c3_w:
	init_c3_w:
	for(int i; i < c3_filter_width; i++){
		for(int j=0; j<c3_filter; j++){
			for(int k = 0; k < 25; k++){
#pragma HLS pipeline II=1
				WBRAM[j][i][k] = filter_w_c3[i*16*25+j*25+k];
			}
		}
	}

	copy_bias_C3:
	for(int i = 0; i < c3_filter; i++){
#pragma HLS pipeline II=1
		biasBRAM[i] = filter_b_c3[i];
	}

	for(int row_k = 0; row_k<5; row_k++){
		for(int col_k = 0; col_k<5; col_k++){
			for(int row = 0; row < c3_output_size; row++){
				for(int col = 0; col < c3_output_size; col++){

					DEPTH_OUT:
					for(int depth_out = 0; depth_out < c3_output_width; depth_out++){
#pragma HLS unroll factor=C2_N_PE
#pragma HLS pipeline II=1
						float mult[c3_filter_width];
#pragma HLS array_partition variable=mult complete dim=0
						float acc=0;
						DEPTH_IN:
						for (int depth_in = 0; depth_in < c3_filter_width; depth_in++){
#pragma HLS unroll
							if(connection_table[depth_out*16+depth_in]){
								mult[depth_in] = maxp2_out[depth_in][row+row_k][col+col_k]*WBRAM[depth_out][depth_in][row_k*5+col_k];
							}else{
								mult[depth_in] = 0;
							}
						}

						acc = (mult[0]+mult[1])+(mult[2]+mult[3])+(mult[4]+mult[5]);
						if(col_k==0 && row_k==0){
							OBRAM[depth_out][row*10+col] = acc;
						}else{
							OBRAM[depth_out][row*10+col] += acc;
						}
					}
				}
			}
		}
	}

	copy_output_C3:
	for(int j = 0; j<c3_output_width; j++){
		int depth_offset = j*100;
		for(int i = 0; i < c3_output_size; i++){
			for(int k = 0; k < c3_output_size; k++){
#pragma HLS pipeline II=1
				conv3_out[j][i][k] = _tanh(OBRAM[j][i*10+k]+biasBRAM[j]);
			}
		}
	}

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	maxpooling4(conv3_out,maxp4_out);

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ c5
	// conv5(maxp4_out,filter_w_c5,filter_b_c5,conv5_out);

	float WBRAM_C5[fc_output_width][c3_output_width][c3_filter_size*c3_filter_size];
#pragma HLS array_partition variable=WBRAM_C5 complete dim=2

	float biasBRAM_C5[fc_output_width];
	float OBRAM_C5[fc_output_width];

	copy_w_c5:
	for(int i = 0; i < c3_output_width; i++){
		for(int j = 0; j < fc_output_width; j++){
			for(int k = 0; k < 25; k++){
#pragma HLS pipeline
				WBRAM_C5[j][i][k] = filter_w_c3[i*120*25+j*25+k];
			}
		}
	}

	copy_bias_C5:
	for(int i = 0; i < 120; i++){
#pragma HLS pipeline II=1
		biasBRAM_C5[i] = filter_b_c5[i];
	}

	for(int row_k =0; row_k<5; row_k++){
		for(int col_k = 0; col_k < 5; col_k++){
			for(int co = 0; co < 120; co++){
#pragma HLS pipeline II=1
				float mult[16];
#pragma HLS array_partition variable=mult complete dim=0
				float acc[4];
#pragma HLS array_partition variable=acc complete dim=0
				float result = 0;
				for(int ci = 0; ci < 16; ci++){
#pragma HLS unroll
					mult[ci] = maxp4_out[ci][row_k][col_k]*WBRAM_C5[co][ci][row_k*5+col_k];
				}

				for(int i = 0, ii=0; i < 4; i++, ii+=4){
#pragma HLS unroll
					acc[i] = (mult[ii]+mult[ii+1])+(mult[ii+2]+mult[ii+3]);
				}
				result = (acc[0]+acc[1])+(acc[2]+acc[3]);
				if(col_k==0 && row_k == 0){
					OBRAM_C5[co] = result;
				}else{
					OBRAM_C5[co] += result;
				}

			}
		}
	}

	for(int i = 0; i < 120; i++){
#pragma HLS pipeline II=1
		conv5_out[i] = _tanh(OBRAM_C5[i]+biasBRAM_C5[i]);
	}

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	output6(conv5_out,filter_w_fc,filter_b_fc,final_layer_out);

	for(int i = 0; i < output_width;i++){
		output[i].data = final_layer_out[i];
		bool last = false;
		if(i == 9) last = true;
		output[i].tlast = last;

	}
/*	for(int i = 0; i < 16;i++){
		for(int j = 0; j < 5;j++){
			for(int z = 0; z<5;z++){
				output[i*25+j*5+z].data = maxp4_out[i][j][z];
				printf("%f\n",output[i*25+j*5+z].data);
				bool last = false;
				if(i == 15 && j == 4 && z == 4)
					last = true;
				output[i*25+j*5+z].tlast = last;
			}
		}
	}*/
/*
	for(int i = 0; i < 120;i++){
		output[i].data = conv5_out[i];
		printf("%f\n",output[i].data);
		bool last = false;
		if(i == 119) last = true;
		output[i].tlast = last;
	}
*/

}
void conv1(float input_image_bram[c1_input_size][c1_input_size], float filter_w_c1[c1_filter*c1_filter_size*c1_filter_size],
		float filter_b_c1[c1_output_width], float (*conv1_out)[c1_output_size][c1_output_size]){
	float temp = 0;
//	convolution_kernel: // 6
//
//	 for (int row_k =0; row_k < 5; row_k++){
//
//	 	for(int col_k=0;col_k<5;col_k++){
//
//	 		for(int row = 0; row < c1_output_size; row++){
//
//	 			conv1_label0:for(int col = 0; col < c1_output_size; col++){
//	 				// pipe
//	 				float input_pixel = input_image_bram[row+row_k][col+col_k];
//	 				float mult[6];
//	 				// array partition
//	 				conv1_label1:for (int co = 0; co < 6; co++){
//	 					// unroll
//	 					mult[co] = input_pixel*filter_w_c1[co*25+row_k*5+col_k];
//	 					if(row_k==0&&col_k==0){
//	 						conv1_out[co][row][col] = mult[co];
//	 					}else{
//	 						conv1_out[co][row][col] += mult[co];
//	 					}
//
//	 				}
//
//
//	 			}
//	 		}
//
//	 	}
//	 }
//
//	 for(int co = 0; co < 6; co++){
//	 	for(int row = 0; row < c1_output_size; row++){
//	 		for(int col = 0; col < c1_output_size; col++){
//	 			conv1_out[co][row][col] = _tanh(conv1_out[co][row][col] + filter_b_c1[co]);
//	 		}
//	 	}
//	 }


	for(int kernel = 0; kernel < c1_filter; kernel++){

		convolution_input_row_loop: // 32
		for(int input_row = 0; input_row < c1_output_size; input_row++){
			convolution_input_col_loop: // 32
			for(int input_col = 0; input_col < c1_output_size; input_col++){
				convolution_kernel_row: // 5
				// sliding filter

				temp = 0;
				for(int kernel_row = 0; kernel_row < c1_filter_size; kernel_row++){
					convolution_kernel_col: // 5
					for(int kernel_col = 0; kernel_col < c1_filter_size; kernel_col++){

						temp += filter_w_c1[kernel*c1_filter_size*c1_filter_size+kernel_row*c1_filter_size+kernel_col]* input_image_bram[input_row+kernel_row][input_col+kernel_col];
					}
				}
				conv1_out[kernel][input_row][input_col] = _tanh(temp + filter_b_c1[kernel]);
				//printf("%f\n",conv1_out[kernel][input_row][input_col]);
			}
		}
	}

}

				// 6               4                6              2                2          2
void maxpooling2(
		float (*input)[c1_output_size][c1_output_size],float (*output)[s2_output_size][s2_output_size]
)
{
	int stride_row = c1_output_size/s2_stride;
	int stride_col = c1_output_size/s2_stride;
input_image_travase:
	for(int num = 0; num < c1_output_width; num++){
		row:
		for(int row = 0; row < stride_row; row++){
			col:
			for(int col = 0; col < stride_col; col++){
				int big_row = row*s2_stride;
				int big_col = col*s2_stride;
				float max = input[num][big_row][big_col];
				pooling_row:
				for(int row_P = 0; row_P <  s2_pooling_size; row_P ++){
					pooling_col:
					for(int col_P = 0; col_P <  s2_pooling_size; col_P ++){
						if (input[num][big_row+row_P][big_col+col_P] > max)
							max = input[num][big_row+row_P][big_col+col_P];
					}
				}
				output[num][row][col] = _tanh(max);
			}
		}
	}
}


void conv3(	float (*input)[s2_output_size][s2_output_size],
			float weight[c3_filter_width*c3_filter_size*c3_filter_size*c3_filter],
			float bias[c3_filter],
			float (*output)[c3_output_size][c3_output_size])
{
	static bool connection_table[6*16] = {
	                true, false, false, false, true, true, true, false, false, true, true, true, true, false, true, true,
	                true, true, false, false, false, true, true, true, false, false, true, true, true, true, false, true,
	                true, true, true, false, false, false, true, true,true, false, false, true, false, true, true,true,
	                false, true, true, true, false, false, true, true, true, true, false, false, true, false, true, true,
	                false, false, true, true, true, false, false, true, true, true, true, false, true, true, false, true,
	                false, false, false, true, true, true, false, false, true, true, true, true, false, true, true, true
	};

	for( int ow = 0; ow < c3_output_width; ow++){
		int flag = 0;
		for(int iw = 0; iw < c3_input_width; iw ++){
			int index = iw*c3_output_width + ow;
			if(!connection_table[index])
				continue;
			for (int i = 0; i < c3_output_size; i++) {
				for (int j = 0; j < c3_output_size; j++) {
					float sum = 0;
					for (int n = 0; n < c3_filter_size; n++) {
						for (int m = 0; m < c3_filter_size; m++) {
							sum += input[iw][i+n][j+m] * weight[index*c3_filter_size*c3_filter_size+n*c3_filter_size + m];
						}
					}
					//output[ow][i][j] += sum;
					if(flag == 0)
					{
						output[ow][i][j] = sum;
					} else {
						output[ow][i][j] += sum;
					}
				}
			}
			flag ++;

		}
		for(int k = 0; k< c3_output_size;k++){
			for(int m = 0; m < c3_output_size;m++){
				output[ow][k][m] = _tanh(output[ow][k][m]+bias[ow]);

			}
		}
	}
}

void maxpooling4(
		float (*input)[c3_output_size][c3_output_size],float (*output)[s4_output_size][s4_output_size]
)
{
	int stride_row = c3_output_size/s4_stride;
	int stride_col = c3_output_size/s4_stride;
	input_image_travase:
	for(int num = 0; num < c3_output_width; num++){
		row:
		for(int row = 0; row < stride_row; row++){
			col:
			for(int col = 0; col < stride_col; col++){
				int big_row = row*s4_stride;
				int big_col = col*s4_stride;
				float max = input[num][big_row][big_col];
				pooling_row:
				for(int row_P = 0; row_P <  s4_pooling_size; row_P ++){
					pooling_col:
					for(int col_P = 0; col_P <  s4_pooling_size; col_P ++){
						if (input[num][big_row+row_P][big_col+col_P] > max)
							max = input[num][big_row+row_P][big_col+col_P];
					}
				}
				output[num][row][col] = _tanh(max);

			}
		}
	}
}


void conv5(float (*input)[s4_output_size][s4_output_size],
		float weight[fc_output_width*s4_output_size*s4_output_size*c3_output_width],
		float bias[fc_output_width],
		float output[fc_output_width])
{


//	for(int row_k=0;row_k<5;row_k++){
//		for(int col_k=0;col_k<5;col_k++){
//			conv5_label2:
//			for(int co=0;co<fc_output_width;co++){
//				#pragma HLS pipeline II=1
//
//				float mult[16];
//				#pragma HLS array_partition variable=mult complete dim=0
//				float acc[4];
//				#pragma HLS array_partition variable=acc complete dim=0
//				float result = 0;
//
//				conv5_label3:for(int ci = 0; ci<16; ci++){
//					// unroll
//				#pragma HLS unroll
//					mult[ci] = input[ci][row_k][col_k]*weight[co*400+ci*25+row_k*5+col_k];
//				}
//
//				conv5_label4:for(int i = 0, ii=0; i<4; i++, ii+=4){
//				#pragma HLS unroll
//					//unroll
//					acc[i] = (mult[ii]+mult[ii+1])+(mult[ii+2]+mult[ii+3]);
//				}
//
//				result = (acc[0]+acc[1])+(acc[2]+acc[3]);
//				if(col_k == 0 && row_k==0){
//					output[co] = result;
//				}else{
//					output[co] += result;
//				}
//			}
//		}
//	}
//
//	for(int i = 0; i < 120; i++){
//		//pipeline (optional)
//		output[i] = _tanh(output[i]+bias[i]);
//	}
	 for(int feature_map = 0; feature_map < fc_output_width; feature_map++){
	 	float sum = 0;
	 	for(int width = 0; width < c3_output_width ; width++){
	 		int index = width * fc_output_width + feature_map;
	 		for(int kernel_row = 0; kernel_row < s4_output_size; kernel_row++){
	 			for(int kernel_col = 0; kernel_col < s4_output_size; kernel_col++){
	 				sum += input[width][kernel_row][kernel_col]*weight[index*25+ kernel_row * s4_output_size+kernel_col];


	 			}
	 		}
	 	}
	 	output[feature_map] = _tanh(sum + bias[feature_map]);


	 }

}

void output6(float input[fc_output_width],float weight[fc_output_width*output_width],float bias[output_width],float output[output_width])
{
	for(int i = 0 ;i < output_width; i++){
		float sum = 0;
		for(int j = 0; j < fc_output_width ; j++){
			sum += input[j]*weight[j*output_width + i];
		}
		output[i] = _tanh(sum+bias[i]);

	}
}



















