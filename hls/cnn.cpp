#include "cnn.h"
#include <math.h>
#include <hls_stream.h>


float _tanh(float x){
	#pragma HLS INLINE
	float exp2x =  expf(2*x)+1;
	return (exp2x-2)/(exp2x);
}



void c1(
		fp_data input_image[c1_input_width][c1_input_width],
		float filter_w[c1_filter*c1_filter_size*c1_filter_size],
		float filter_b[c1_filter],
		fp_data output[c1_output_width*c1_output_size*c1_output_size]
		){

	#pragma HLS INTERFACE axis port=output
	#pragma HLS INTERFACE axis port=input_image
	#pragma HLS INTERFACE s_axilite port=return bundle=CRTL_BUS
	#pragma HLS INTERFACE s_axilite port=filter_w bundle=KERNEL_BUS
	#pragma HLS INTERFACE s_axilite port=filter_b bundle=KERNEL_BUS

	float input_image_bram[c1_input_width][c1_input_width];
#pragma HLS RESOURCE variable=input_image_bram core=RAM_1P_BRAM
	float conv_out[c1_filter][c1_output_size][c1_output_size];
#pragma HLS RESOURCE variable=conv_out core=RAM_1P_BRAM
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
	image:
	init_image_col:
	for(int row = 0; row < c1_input_width; row++){
		init_image_row:
		for(int col = 0; col < c1_input_width; col++){
			input_image_bram[row][col] = input_image[row][col].data;
		}
	}
	float temp = 0;
	convolution_kernel: // 6
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
						#pragma HLS PIPELINE
						temp += filter_w[kernel*c1_filter_size*c1_filter_size+kernel_row*c1_filter_size+kernel_col]* input_image_bram[input_row+kernel_row][input_col+kernel_col];
					}
				}
				conv_out[kernel][input_row][input_col] = temp + filter_b[kernel];
			}
		}
	}

	for(int kernel = 0; kernel < c1_output_width; kernel++){
			activition_row:
			for(int row = 0; row < c1_output_size; row++){
				activition_col:
				for(int col = 0; col < c1_output_size; col++){
					output[kernel*c1_output_size*c1_output_size+row*c1_output_size+col].data = _tanh(conv_out[kernel][row][col]);
					//if (kernel == 5 && col == 27 && row == 27){
					bool last_ret = false;
					if (kernel == (c1_output_width-1) && col == (c1_output_size-1) && row == (c1_output_size-1)){
						last_ret = true;
					}
					output[kernel*c1_output_size*c1_output_size+row*c1_output_size+col].last = last_ret ;

					//	output[kernel*c1_output_size*c1_output_size+col*c1_output_size+row].last = 1;

				}
			}
	}

}


























/*
// C1 layer
// input 32*32
// kernel 6
// kernel size 5*5
// output 6*28*28
void c1(float input_image[c1_input_width*c1_input_width],
		float filter_w[c1_filter*c1_filter_size*c1_filter_size],
		float filter_b[c1_filter],
		float output[c1_output_width*c1_output_size*c1_output_size]
		float input_image_bram[c1_input_width*c1_input_width],
		float filter_w_bram[c1_filter*c1_filter_size*c1_filter_size],
		float filter_b_bram[c1_filter],
		float output[c1_output_width*c1_output_size*c1_output_size]
		){
	float conv_out[c1_filter][c1_output_size][c1_output_size];
	float input_image_bram[c1_input_width][c1_input_width];
	float filter_w_bram[c1_filter][c1_filter_size][c1_filter_size];
	float filter_b_bram[c1_filter];
	float conv_out[c1_filter][c1_output_size][c1_output_size];
	/////// Do we need copy input?(kernel weight,bias for future reuse)///////
	/////// How to copy weight,bias just once?                         ///////
	initial_parameter:
		init_kernel: // 6
			for(int kernel = 0; kernel < c1_filter; kernel++){
				init_col:
				for(int col = 0; col < c1_filter_size; col++){
					init_row:
					for(int row = 0; row < c1_filter_size; row++){
						filter_w_bram[kernel][row][col] = filter_w[kernel*c1_filter_size*c1_filter_size + col*c1_filter_size + row];
					}
				}
			}
		bias:
			for(int i = 0; i < c1_filter; i++){
				filter_b_bram[i] = filter_b[i];
			}
		image:
			init_image_col:
			for(int row = 0; row < c1_input_width; row++){
				init_image_row:
				for(int col = 0; col < c1_input_width; col++){
					input_image_bram[row][col] = input_image[row*c1_input_width+col];
				}
			}

	////////////////////////////////////
	/////// 	Convolution 	 ///////
	////////////////////////////////////
	float temp = 0;
	convolution_kernel: // 6
	for(int kernel = 0; kernel < c1_filter; kernel++){
		convolution_input_col_loop: // 32
		for(int input_col = 0; input_col < c1_output_size; input_col++){
			convolution_input_row_loop: // 32
			for(int input_row = 0; input_row < c1_output_size; input_row++){
				convolution_kernel_col: // 5
				// sliding filter
				temp = 0;
				for(int kernel_col = 0; kernel_col < c1_filter_size; kernel_col++){
					convolution_kernel_row: // 5
					for(int kernel_row = 0; kernel_row < c1_filter_size; kernel_row++){
						temp += filter_w_bram[kernel][kernel_row][kernel_col]* input_image_bram[input_row+kernel_row][input_col+kernel_col];
					}
				}
				output[kernel][input_row][input_col] = _tanh(temp + filter_b_bram[kernel]);
			}
		}
	}
	///////////////////////////////////
	/////// 	Activition 	    ///////
	///////////////////////////////////
	activition_non_linear_function:
	activition_kernel:
	for(int kernel = 0; kernel < c1_filter; kernel++){
		activition_col:
		for(int col = 0; col < c1_output_size; col++){
			activition_row:
			for(int row = 0; row < c1_output_size; row++){
				output_bram[kernel*c1_output_size*c1_output_size+col*c1_output_size+row] = _tanh(conv_out[kernel][row][col]);
			}
		}
	}
}
void c2(float input[c2_input_width][c2_input_size][c2_input_size],
		float filter_w[c2_filter][c2_filter_width][c2_filter_size][c2_filter_size],
		float filter_b[c2_filter],
		float output[c2_output_width][c2_output_size][c2_output_size]
		){
	float filter_w_bram[c2_filter][c2_filter_width][c2_filter_size][c2_filter_size];
	float filter_b_bram[c2_filter];
	float conv_out[c2_output_width][c2_output_size][c2_output_size];
	///////////////////////////////////
	/////// Copy weight bias    ///////
	///////////////////////////////////
	initial_parameter:
			init_kernel: // 16
			for(int filter = 0; filter < c2_filter; c2_filter++){
				kernel_width_copy://6
				for(int kernel_width = 0; kernel_width < c2_filter_width; kernel_width++){
					kerner_row://5
					for(int row = 0; row < c2_filter_size; row++){
						kernel_col://5
						for(int col = 0; col < c2_filter_size; col++){
							filter_w_bram[filter][kernel_width][row][col] = filter_w[filter][kernel_width][row][col];
						}
					}
				}
			}
			init_bias:
				for(int bias; bias < c2_filter; bias++)
					filter_b_bram[bias] = filter_b[bias];
	////////////////////////////////////
	/////// 	Convolution 	 ///////
	////////////////////////////////////
	float temp = 0;
	//convolution_kernel:
	//16
	c2_kernel:
	for(int kernel = 0; kernel < c2_filter; kernel ++){
		c2_intput_row:
		for( int input_row = 0; input_row < c2_input_size; input_row ++ ){
			c2_input_col:
			for(int input_col = 0; input_col < c2_input_size; input_col ++){
				c2input:
				temp = 0;
				for(int input_width = 0; input_width < c2_input_width;input_width++){
					c2_kernel_row:
					for(int kernel_row = 0; kernel_row < c2_filter_size; kernel_row ++){
						c2_kernel_col:
						for (int kernel_col = 0; kernel_col < c2_filter_size; kernel_col ++){
							temp += filter_w_bram[kernel][input_width][kernel_row][kernel_col]*input[input_width][input_row+kernel_row][input_col+kernel_col];
						}
					}

				}
				conv_out[kernel][input_row][input_col] = temp + filter_b_bram[kernel];
			}
		}
	}
	///////////////////////////////////
	/////// 	Activition 	    ///////
	///////////////////////////////////
	activition_non_linear_function:
		activition_kernel:
		for(int kernel = 0; kernel < c2_filter; kernel++){
			activition_col:
			for(int row = 0; row < c2_output_size; row++){
				activition_row:
				for(int col = 0; col < c2_output_size; col++){
					output[kernel][row][col] = _tanh(conv_out[kernel][row][col]);
				}
			}
		}

}

void c3(float input[c3_input_width][c3_input_size][c3_input_size],
		float filter_w[c3_filter][c3_filter_width][c3_filter_size][c3_filter_size],
		float filter_b[c3_filter],
		float output[c3_output_width][c3_output_size][c3_output_size]){
	float filter_w_bram[c3_filter][c3_filter_width][c3_filter_size][c3_filter_size];
	float filter_b_bram[c3_filter];
	float conv_out[c3_output_width];
	///////////////////////////////////
	/////// Copy weight bias    ///////
	///////////////////////////////////
	initial_parameter:
	init_kernel: // 16
	for(int filter = 0; filter < c3_filter; c2_filter++){
		kernel_width_copy://6
		for(int kernel_width = 0; kernel_width < c3_filter_width; kernel_width++){
			kerner_row://5
			for(int row = 0; row < c3_filter_size; row++){
				kernel_col://5
				for(int col = 0; col < c3_filter_size; col++){
					filter_w_bram[filter][kernel_width][row][col] = filter_w[filter][kernel_width][row][col];
				}
			}
		}
	}
	init_bias:
		for(int bias; bias < c2_filter; bias++)
			filter_b_bram[bias] = filter_b[bias];

	////////////////////////////////////
	/////// 	Convolution 	 ///////
	////////////////////////////////////
		float temp = 0;
		//convolution_kernel:
		//16
		c2_kernel:
		for(int kernel = 0; kernel < c3_filter; kernel ++){
			c2input:
			temp = 0;
			for(int input_width = 0; input_width < c3_input_width;input_width++){
				c2_kernel_row:
				for(int kernel_row = 0; kernel_row < c3_filter_size; kernel_row ++){
					c2_kernel_col:
					for (int kernel_col = 0; kernel_col < c3_filter_size; kernel_col ++){
						temp += filter_w_bram[kernel][input_width][kernel_row][kernel_col]*input[input_width][kernel_row][kernel_col];
					}
				}

			}
			conv_out[kernel] = temp + filter_b_bram[kernel];
		}

		///////////////////////////////////
		/////// 	Activition 	    ///////
		///////////////////////////////////
		for(int kernel = 0; kernel < c3_filter; kernel++){
			output[kernel] = _tanh(conv_out[kernel]);
		}
}*/
