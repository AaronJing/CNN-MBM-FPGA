
struct fp_data {
	float data;
	bool tlast;
};
///// first convolution layer /////
#define c1_input_width 1
#define c1_input_size 32
#define c1_filter 6
///// 2D kernel 5*5			  /////
#define c1_filter_size 5
#define c1_filter_width 1
///// 3D output 28*28*6       /////
#define c1_output_size 28
#define c1_output_width 6

//#define c1_stride 1
//#define c1_padding 0
///// end of first convolution layer/////


///// first max pooling layer layer /////
#define s2_output_width 6
#define s2_output_size 14
#define s2_stride 2
#define s2_pooling_size 2


///// second convolution layer /////
#define c3_filter 16
///// 16 * (3D kernel 5*5*6)		   /////
///// total kernel 16(5*5*6)   /////
///// real lenet is (5*5*3+1)*6
///// +(5*5*4+1)*9 +(5*5*6+1) + 1
///// 1517
#define c3_filter_size 5
#define c3_filter_width 6
///// 3D input 6*14*14		   /////
#define c3_input_size 14
#define c3_input_width 6
///// 3D output 16*10*10	   /////
#define c3_output_size 10
#define c3_output_width 16

//#define c2_stride 1
//#define c2_padding 0
///// end of second convolution layer /////

///// second max pooling layer layer /////
#define s4_output_size 5
#define s4_stride 2
#define s4_pooling_size 2

#define fc_output_size 1
#define fc_output_width 120

#define output_width 10
#define output_size 1

void conv1(float input_image_bram[c1_input_size][c1_input_size], float filter_w_c1[c1_filter*c1_filter_size*c1_filter_size],
		float filter_b_c1[c1_output_width], float (*conv1_out)[c1_output_size][c1_output_size]);
void maxpooling2(	float (*input)[c1_output_size][c1_output_size],
					float (*output)[s2_output_size][s2_output_size]);
void maxpooling4(
		float (*input)[c3_output_size][c3_output_size],float (*output)[s4_output_size][s4_output_size]
);
void conv5(float (*input)[s4_output_size][s4_output_size],
		float weight[fc_output_width*s4_output_size*s4_output_size*c3_output_width],
		float bias[fc_output_width],

		float output[fc_output_width]
);
void conv3(	float (*input)[s2_output_size][s2_output_size],
			float weight[c3_filter_width*c3_filter_size*c3_filter_size*c3_filter],
			float bias[c3_filter],
			float (*output)[c3_output_size][c3_output_size]);
void output6(float input[fc_output_width],float weight[fc_output_width*output_width],float bias[output_width],float output[output_width]);
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
		fp_data output[120]
		);







/*
///// third convolution layer /////
#define c3_filter 120
///// 120*(3D kernel 16*5*5)  /////
#define c3_filter_width 16
#define c3_filter_size 5

///// 3D input 16*5*5		  /////
#define c3_input_size 5
#define c3_input_width 16

///// 3D output 120*1*1	   /////
#define c3_output_size 1
#define c3_output_width 120
//#define c3_stride 1
//#define c3_padding 0
///// end of third convolution layer /////
*/





