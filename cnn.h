
///// first convolution layer /////
#define c1_input_width 32
#define c1_filter 6
///// 2D kernel 5*5			  /////
#define c1_filter_size 5
///// 3D output 28*28*6       /////
#define c1_output_size 28
#define c1_output_width 6

//#define c1_stride 1
//#define c1_padding 0
///// end of first convolution layer/////



///// second convolution layer /////
#define c2_filter 16
///// 16 * (3D kernel 5*5*6)		   /////
///// total kernel 16(5*5*6)   /////
///// real lenet is (5*5*3+1)*6
///// +(5*5*4+1)*9 +(5*5*6+1) + 1
///// 1517
#define c2_filter_size 5
#define c2_filter_width 6
///// 3D input 6*14*14		   /////
#define c2_input_size 14
#define c2_input_width 6
///// 3D output 16*10*10	   /////
#define c2_output_size 10
#define c2_output_width 16

//#define c2_stride 1
//#define c2_padding 0
///// end of second convolution layer /////



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
