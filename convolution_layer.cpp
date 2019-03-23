#include "cnn.h"
#include "hls_math.h"
float _tanh(float x){
#pragma HLS INLINE
	float exp2x = expf(2*x)+1;
	return (exp2x-2)/(exp2x);
}
// C1 layer
// input 32*32
// kernel 6
// kernel size 5*5
// output 6*28*28
void c1(float input_image[c1_input_width*c1_input_width],
		float filter_w[c1_filter*c1_filter_size*c1_filter_size],
		float filter_b[c1_filter],
		float output[c1_output_feature_num*c1_output_size*c1_output_size]
		){
	float input_image_bram[c1_input_width][c1_input_width];
	float filter_w_bram[c1_filter][c1_filter_size][c1_filter_size];
	float filter_b_bram[c1_filter];
	float conv_out[c1_filter][c1_output_size][c1_output_size];
	/////// Do we need copy input?(kernel weight,bias for future reuse)///////
	/////// How to copy weight,bias just once?                         ///////
	initial_parameter:
		kernel: // 6
			for(int kernel = 0; kernel < c1_filter; kernel++){
				col:
				for(int col = 0; col < c1_filter_size; col++){
					row:
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
			col:
			for(int col = 0; col < c1_input_width; col++){
				row:
				for(int row = 0; row < c1_input_width; row++){
					input_image_bram[row][col] = input_image[row][col];
				}
			}

	////////////////////////////////////
	/////// 	Convolution 	 ///////
	////////////////////////////////////
	float temp;
	kernel: // 6
	for(int kernel = 0; kernel < c1_filter; kernel++){
		input_col_loop: // 32
		for(int input_col = 0; input_col < c1_output_size; input_col++){
			input_row_loop: // 32
			for(int input_row = 0; input_row < c1_output_size; input_row++){
				kernel_col: // 5
				// sliding filter
				temp = 0;
				for(int kernel_col = 0; kernel_col < c1_filter_size; kernel_col++){
					kernel_row: // 5
					for(int kernel_row = 0; kernel_row < c1_filter_size; kernel_row++){
						temp += filter_w_bram[kernel][kernel_row][kernel_col]* input_image_bram[input_row+kernel_row][input_col+kernel_col];
					}
				}
				conv_out[kernel][input_row][input_col] = temp + filter_b_bram[kernel];
			}
		}
	}
	///////////////////////////////////
	/////// 	Activition 	    ///////
	///////////////////////////////////
	non_linear_function:
	kernel:
	for(int kernel = 0; kernel < c1_filter; kernel++){
		col:
		for(int col = 0; col < c1_output_size; col++){
			row:
			for(int row = 0; row < c1_output_size; row++){
				output[kernel*c1_output_size*c1_output_size+col*c1_output_size+row] = _tanh(conv_out[kernel][row][col]);
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
}
