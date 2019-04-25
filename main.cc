/*
 * Empty C++ Application
 */
/*
 * Empty C++ Application
 */
#include <stdio.h>
#include "xtop.h"
#include "xaxidma.h"
#include "cnn.h"
#include <math.h>
#include <xtime_l.h>


#define mem_base_addr 0x01000000

#define tx_buffer_base (mem_base_addr + 0x00100000)

#define rx_buffer_base (mem_base_addr + 0x01500000)


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
	#include "c3w-ro.txt"
};
float c3_w_sw[2400]={
	#include "c3w.txt"
};
float c3_b[16]={
	#include "c3b.txt"
};
float c5_w[48000]={
    #include "c5w-ro.txt"
};
float c5_w_sw[48000]={
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
float *m_dma_buffer_RX = (float*)rx_buffer_base;



float find_max(float* input);

XAxiDma axiDma;
int initDMA()
{
	XAxiDma_Config *CfgPtr;
	CfgPtr = XAxiDma_LookupConfig(XPAR_AXI_DMA_0_DEVICE_ID);
	XAxiDma_CfgInitialize(&axiDma,CfgPtr);

	// Disable interrupts
	XAxiDma_IntrDisable(&axiDma,XAXIDMA_IRQ_ALL_MASK,XAXIDMA_DEVICE_TO_DMA);
	XAxiDma_IntrDisable(&axiDma,XAXIDMA_IRQ_ALL_MASK,XAXIDMA_DMA_TO_DEVICE);

	return XST_SUCCESS;
}
XTop doCnn;


int initTOP()
{
	int status;

	XTop_Config *doCnn_cfg;
	doCnn_cfg = XTop_LookupConfig(XPAR_TOP_0_DEVICE_ID);
	if (!doCnn_cfg)
	{
		printf("Error loading config for doHist_cfg\n");
	}
	status = XTop_CfgInitialize(&doCnn,doCnn_cfg);
	if (status != XST_SUCCESS)
	{
		printf("Error initializing for doHist\n");
	}

	return status;
}
float sw_top(float *input);
float _tanh(float x){
	float exp2x =  expf(2*x)+1;
	return (exp2x-2)/(exp2x);
//	return atanf(x);
}

int main()
{
	XTime start,end;

	 printf("Starting hardware\n");
	initDMA();
	initTOP();


//	XTop_Write_filter_w_c1_0_Words(&doCnn,0,(int*)c1_w,25);
//	XTop_Write_filter_w_c1_1_Words(&doCnn,0,(int*)c1_w+25,25);
//	XTop_Write_filter_w_c1_2_Words(&doCnn,0,(int*)c1_w+50,25);
//	XTop_Write_filter_w_c1_3_Words(&doCnn,0,(int*)c1_w+75,25);
//	XTop_Write_filter_w_c1_4_Words(&doCnn,0,(int*)c1_w+100,25);
//	XTop_Write_filter_w_c1_5_Words(&doCnn,0,(int*)c1_w+125,25);
//
//	 XTop_Set_filter_b_c1_0(&doCnn,c1_b[0]);
//	 XTop_Set_filter_b_c1_1(&doCnn,c1_b[1]);
//	 XTop_Set_filter_b_c1_2(&doCnn,c1_b[2]);
//	 XTop_Set_filter_b_c1_3(&doCnn,c1_b[3]);
//	 XTop_Set_filter_b_c1_4(&doCnn,c1_b[4]);
//	 XTop_Set_filter_b_c1_5(&doCnn,c1_b[5]);




	// 0 2 4 6 8... 2nd partition row, 1st parition row
	// 1-16set - 1st filter
	// even
//	XTop_Write_filter_w_c3_0_0_Words(&doCnn,0,(int*)c3_w,25);
//	XTop_Write_filter_w_c3_0_0_Words(&doCnn,25,(int*)c3_w+(6*25*2),25);
//	XTop_Write_filter_w_c3_0_0_Words(&doCnn,50,(int*)c3_w+(6*25*4),25);
//	XTop_Write_filter_w_c3_0_0_Words(&doCnn,75,(int*)c3_w+(6*25*6),25);
//	XTop_Write_filter_w_c3_0_0_Words(&doCnn,100,(int*)c3_w+(6*25*8),25);
//	XTop_Write_filter_w_c3_0_0_Words(&doCnn,125,(int*)c3_w+(6*25*10),25);
//	XTop_Write_filter_w_c3_0_0_Words(&doCnn,150,(int*)c3_w+(6*25*12),25);
//	XTop_Write_filter_w_c3_0_0_Words(&doCnn,175,(int*)c3_w+(6*25*14),25);
//
//	// odd
//	XTop_Write_filter_w_c3_1_0_Words(&doCnn,0,(int*)c3_w+(6*25*1),25);
//	XTop_Write_filter_w_c3_1_0_Words(&doCnn,25,(int*)c3_w+(6*25*3),25);
//	XTop_Write_filter_w_c3_1_0_Words(&doCnn,50,(int*)c3_w+(6*25*5),25);
//	XTop_Write_filter_w_c3_1_0_Words(&doCnn,75,(int*)c3_w+(6*25*7),25);
//	XTop_Write_filter_w_c3_1_0_Words(&doCnn,100,(int*)c3_w+(6*25*9),25);
//	XTop_Write_filter_w_c3_1_0_Words(&doCnn,125,(int*)c3_w+(6*25*11),25);
//	XTop_Write_filter_w_c3_1_0_Words(&doCnn,150,(int*)c3_w+(6*25*13),25);
//	XTop_Write_filter_w_c3_1_0_Words(&doCnn,175,(int*)c3_w+(6*25*15),25);
//
//	// 1-16set - 2nd filter
//	// even
//	XTop_Write_filter_w_c3_0_1_Words(&doCnn,0,(int*)c3_w+25,25);
//	XTop_Write_filter_w_c3_0_1_Words(&doCnn,25,(int*)c3_w+(6*25*2)+25,25);
//	XTop_Write_filter_w_c3_0_1_Words(&doCnn,50,(int*)c3_w+(6*25*4)+25,25);
//	XTop_Write_filter_w_c3_0_1_Words(&doCnn,75,(int*)c3_w+(6*25*6)+25,25);
//	XTop_Write_filter_w_c3_0_1_Words(&doCnn,100,(int*)c3_w+(6*25*8)+25,25);
//	XTop_Write_filter_w_c3_0_1_Words(&doCnn,125,(int*)c3_w+(6*25*10)+25,25);
//	XTop_Write_filter_w_c3_0_1_Words(&doCnn,150,(int*)c3_w+(6*25*12)+25,25);
//	XTop_Write_filter_w_c3_0_1_Words(&doCnn,175,(int*)c3_w+(6*25*14)+25,25);
//
//	// odd
//	XTop_Write_filter_w_c3_1_1_Words(&doCnn,0,(int*)c3_w+(6*25*1)+25,25);
//	XTop_Write_filter_w_c3_1_1_Words(&doCnn,25,(int*)c3_w+(6*25*3)+25,25);
//	XTop_Write_filter_w_c3_1_1_Words(&doCnn,50,(int*)c3_w+(6*25*5)+25,25);
//	XTop_Write_filter_w_c3_1_1_Words(&doCnn,75,(int*)c3_w+(6*25*7)+25,25);
//	XTop_Write_filter_w_c3_1_1_Words(&doCnn,100,(int*)c3_w+(6*25*9)+25,25);
//	XTop_Write_filter_w_c3_1_1_Words(&doCnn,125,(int*)c3_w+(6*25*11)+25,25);
//	XTop_Write_filter_w_c3_1_1_Words(&doCnn,150,(int*)c3_w+(6*25*13)+25,25);
//	XTop_Write_filter_w_c3_1_1_Words(&doCnn,175,(int*)c3_w+(6*25*15)+25,25);
//
//	// 1-16set - 3rd filter
//	// even
//	XTop_Write_filter_w_c3_0_2_Words(&doCnn,0,(int*)c3_w+50,25);
//	XTop_Write_filter_w_c3_0_2_Words(&doCnn,25,(int*)c3_w+(6*25*2)+50,25);
//	XTop_Write_filter_w_c3_0_2_Words(&doCnn,50,(int*)c3_w+(6*25*4)+50,25);
//	XTop_Write_filter_w_c3_0_2_Words(&doCnn,75,(int*)c3_w+(6*25*6)+50,25);
//	XTop_Write_filter_w_c3_0_2_Words(&doCnn,100,(int*)c3_w+(6*25*8)+50,25);
//	XTop_Write_filter_w_c3_0_2_Words(&doCnn,125,(int*)c3_w+(6*25*10)+50,25);
//	XTop_Write_filter_w_c3_0_2_Words(&doCnn,150,(int*)c3_w+(6*25*12)+50,25);
//	XTop_Write_filter_w_c3_0_2_Words(&doCnn,175,(int*)c3_w+(6*25*14)+50,25);
//
//	// odd
//	XTop_Write_filter_w_c3_1_2_Words(&doCnn,0,(int*)c3_w+(6*25*1)+50,25);
//	XTop_Write_filter_w_c3_1_2_Words(&doCnn,25,(int*)c3_w+(6*25*3)+50,25);
//	XTop_Write_filter_w_c3_1_2_Words(&doCnn,50,(int*)c3_w+(6*25*5)+50,25);
//	XTop_Write_filter_w_c3_1_2_Words(&doCnn,75,(int*)c3_w+(6*25*7)+50,25);
//	XTop_Write_filter_w_c3_1_2_Words(&doCnn,100,(int*)c3_w+(6*25*9)+50,25);
//	XTop_Write_filter_w_c3_1_2_Words(&doCnn,125,(int*)c3_w+(6*25*11)+50,25);
//	XTop_Write_filter_w_c3_1_2_Words(&doCnn,150,(int*)c3_w+(6*25*13)+50,25);
//	XTop_Write_filter_w_c3_1_2_Words(&doCnn,175,(int*)c3_w+(6*25*15)+50,25);
//
//
//	// 1-16set - 4th filter
//	// even
//	XTop_Write_filter_w_c3_0_3_Words(&doCnn,0,(int*)c3_w+75,25);
//	XTop_Write_filter_w_c3_0_3_Words(&doCnn,25,(int*)c3_w+(6*25*2)+75,25);
//	XTop_Write_filter_w_c3_0_3_Words(&doCnn,50,(int*)c3_w+(6*25*4)+75,25);
//	XTop_Write_filter_w_c3_0_3_Words(&doCnn,75,(int*)c3_w+(6*25*6)+75,25);
//	XTop_Write_filter_w_c3_0_3_Words(&doCnn,100,(int*)c3_w+(6*25*8)+75,25);
//	XTop_Write_filter_w_c3_0_3_Words(&doCnn,125,(int*)c3_w+(6*25*10)+75,25);
//	XTop_Write_filter_w_c3_0_3_Words(&doCnn,150,(int*)c3_w+(6*25*12)+75,25);
//	XTop_Write_filter_w_c3_0_3_Words(&doCnn,175,(int*)c3_w+(6*25*14)+75,25);
//
//	// odd
//	XTop_Write_filter_w_c3_1_3_Words(&doCnn,0,(int*)c3_w+(6*25*1)+75,25);
//	XTop_Write_filter_w_c3_1_3_Words(&doCnn,25,(int*)c3_w+(6*25*3)+75,25);
//	XTop_Write_filter_w_c3_1_3_Words(&doCnn,50,(int*)c3_w+(6*25*5)+75,25);
//	XTop_Write_filter_w_c3_1_3_Words(&doCnn,75,(int*)c3_w+(6*25*7)+75,25);
//	XTop_Write_filter_w_c3_1_3_Words(&doCnn,100,(int*)c3_w+(6*25*9)+75,25);
//	XTop_Write_filter_w_c3_1_3_Words(&doCnn,125,(int*)c3_w+(6*25*11)+75,25);
//	XTop_Write_filter_w_c3_1_3_Words(&doCnn,150,(int*)c3_w+(6*25*13)+75,25);
//	XTop_Write_filter_w_c3_1_3_Words(&doCnn,175,(int*)c3_w+(6*25*15)+75,25);
//
//
//	// 1-16set - 5th filter
//	// even
//	XTop_Write_filter_w_c3_0_4_Words(&doCnn,0,(int*)c3_w+100,25);
//	XTop_Write_filter_w_c3_0_4_Words(&doCnn,25,(int*)c3_w+(6*25*2)+100,25);
//	XTop_Write_filter_w_c3_0_4_Words(&doCnn,50,(int*)c3_w+(6*25*4)+100,25);
//	XTop_Write_filter_w_c3_0_4_Words(&doCnn,75,(int*)c3_w+(6*25*6)+100,25);
//	XTop_Write_filter_w_c3_0_4_Words(&doCnn,100,(int*)c3_w+(6*25*8)+100,25);
//	XTop_Write_filter_w_c3_0_4_Words(&doCnn,125,(int*)c3_w+(6*25*10)+100,25);
//	XTop_Write_filter_w_c3_0_4_Words(&doCnn,150,(int*)c3_w+(6*25*12)+100,25);
//	XTop_Write_filter_w_c3_0_4_Words(&doCnn,175,(int*)c3_w+(6*25*14)+100,25);
//
//	// odd
//	XTop_Write_filter_w_c3_1_4_Words(&doCnn,0,(int*)c3_w+(6*25*1)+100,25);
//	XTop_Write_filter_w_c3_1_4_Words(&doCnn,25,(int*)c3_w+(6*25*3)+100,25);
//	XTop_Write_filter_w_c3_1_4_Words(&doCnn,50,(int*)c3_w+(6*25*5)+100,25);
//	XTop_Write_filter_w_c3_1_4_Words(&doCnn,75,(int*)c3_w+(6*25*7)+100,25);
//	XTop_Write_filter_w_c3_1_4_Words(&doCnn,100,(int*)c3_w+(6*25*9)+100,25);
//	XTop_Write_filter_w_c3_1_4_Words(&doCnn,125,(int*)c3_w+(6*25*11)+100,25);
//	XTop_Write_filter_w_c3_1_4_Words(&doCnn,150,(int*)c3_w+(6*25*13)+100,25);
//	XTop_Write_filter_w_c3_1_4_Words(&doCnn,175,(int*)c3_w+(6*25*15)+100,25);
//
//	// 1-16set - 6th filter
//	// even
//	XTop_Write_filter_w_c3_0_5_Words(&doCnn,0,(int*)c3_w+125,25);
//	XTop_Write_filter_w_c3_0_5_Words(&doCnn,25,(int*)c3_w+(6*25*2)+125,25);
//	XTop_Write_filter_w_c3_0_5_Words(&doCnn,50,(int*)c3_w+(6*25*4)+125,25);
//	XTop_Write_filter_w_c3_0_5_Words(&doCnn,75,(int*)c3_w+(6*25*6)+125,25);
//	XTop_Write_filter_w_c3_0_5_Words(&doCnn,100,(int*)c3_w+(6*25*8)+125,25);
//	XTop_Write_filter_w_c3_0_5_Words(&doCnn,125,(int*)c3_w+(6*25*10)+125,25);
//	XTop_Write_filter_w_c3_0_5_Words(&doCnn,150,(int*)c3_w+(6*25*12)+125,25);
//	XTop_Write_filter_w_c3_0_5_Words(&doCnn,175,(int*)c3_w+(6*25*14)+125,25);
//
//	// odd
//	XTop_Write_filter_w_c3_1_5_Words(&doCnn,0,(int*)c3_w+(6*25*1)+125,25);
//	XTop_Write_filter_w_c3_1_5_Words(&doCnn,25,(int*)c3_w+(6*25*3)+125,25);
//	XTop_Write_filter_w_c3_1_5_Words(&doCnn,50,(int*)c3_w+(6*25*5)+125,25);
//	XTop_Write_filter_w_c3_1_5_Words(&doCnn,75,(int*)c3_w+(6*25*7)+125,25);
//	XTop_Write_filter_w_c3_1_5_Words(&doCnn,100,(int*)c3_w+(6*25*9)+125,25);
//	XTop_Write_filter_w_c3_1_5_Words(&doCnn,125,(int*)c3_w+(6*25*11)+125,25);
//	XTop_Write_filter_w_c3_1_5_Words(&doCnn,150,(int*)c3_w+(6*25*13)+125,25);
//	XTop_Write_filter_w_c3_1_5_Words(&doCnn,175,(int*)c3_w+(6*25*15)+125,25);
//
//	XTop_Set_filter_b_c3_0(&doCnn,c3_b[0]);
//	XTop_Set_filter_b_c3_1(&doCnn,c3_b[1]);
//	XTop_Set_filter_b_c3_2(&doCnn,c3_b[2]);
//	XTop_Set_filter_b_c3_3(&doCnn,c3_b[3]);
//	XTop_Set_filter_b_c3_4(&doCnn,c3_b[4]);
//	XTop_Set_filter_b_c3_5(&doCnn,c3_b[5]);
//	XTop_Set_filter_b_c3_6(&doCnn,c3_b[6]);
//	XTop_Set_filter_b_c3_7(&doCnn,c3_b[7]);
//	XTop_Set_filter_b_c3_8(&doCnn,c3_b[8]);
//	XTop_Set_filter_b_c3_9(&doCnn,c3_b[9]);
//	XTop_Set_filter_b_c3_10(&doCnn,c3_b[10]);
//	XTop_Set_filter_b_c3_11(&doCnn,c3_b[11]);
//	XTop_Set_filter_b_c3_12(&doCnn,c3_b[12]);
//	XTop_Set_filter_b_c3_13(&doCnn,c3_b[13]);
//	XTop_Set_filter_b_c3_14(&doCnn,c3_b[14]);
//	XTop_Set_filter_b_c3_15(&doCnn,c3_b[15]);
//
//	//XTop_Write_filter_b_c3_Words(&doCnn,0,(int*)c3_b,16);
//	for(int i = 0; i < 120; i ++){
//		XTop_Write_filter_w_c5_0_Words(&doCnn,i*25,(int*)c5_w+(i*16*25),25);
//	}
//	for(int i = 0; i < 120; i ++){
//			XTop_Write_filter_w_c5_1_Words(&doCnn,i*25,(int*)c5_w+(i*16*25+25),25);
//		}
//	for(int i = 0; i < 120; i ++){
//			XTop_Write_filter_w_c5_2_Words(&doCnn,i*25,(int*)c5_w+(i*16*25+50),25);
//		}
//
//	for(int i = 0; i < 120; i ++){
//			XTop_Write_filter_w_c5_3_Words(&doCnn,i*25,(int*)c5_w+(i*16*25+75),25);
//		}
//	for(int i = 0; i < 120; i ++){
//			XTop_Write_filter_w_c5_4_Words(&doCnn,i*25,(int*)c5_w+(i*16*25+100),25);
//		}
//	for(int i = 0; i < 120; i ++){
//			XTop_Write_filter_w_c5_5_Words(&doCnn,i*25,(int*)c5_w+(i*16*25+125),25);
//		}
//	for(int i = 0; i < 120; i ++){
//			XTop_Write_filter_w_c5_6_Words(&doCnn,i*25,(int*)c5_w+(i*16*25+150),25);
//		}
//	for(int i = 0; i < 120; i ++){
//			XTop_Write_filter_w_c5_7_Words(&doCnn,i*25,(int*)c5_w+(i*16*25+175),25);
//		}
//	for(int i = 0; i < 120; i ++){
//			XTop_Write_filter_w_c5_8_Words(&doCnn,i*25,(int*)c5_w+(i*16*25+200),25);
//		}
//	for(int i = 0; i < 120; i ++){
//			XTop_Write_filter_w_c5_9_Words(&doCnn,i*25,(int*)c5_w+(i*16*25+225),25);
//		}
//	for(int i = 0; i < 120; i ++){
//			XTop_Write_filter_w_c5_10_Words(&doCnn,i*25,(int*)c5_w+(i*16*25+250),25);
//		}
//	for(int i = 0; i < 120; i ++){
//			XTop_Write_filter_w_c5_11_Words(&doCnn,i*25,(int*)c5_w+(i*16*25+275),25);
//		}
//	for(int i = 0; i < 120; i ++){
//			XTop_Write_filter_w_c5_12_Words(&doCnn,i*25,(int*)c5_w+(i*16*25+300),25);
//		}
//	for(int i = 0; i < 120; i ++){
//			XTop_Write_filter_w_c5_13_Words(&doCnn,i*25,(int*)c5_w+(i*16*25+325),25);
//		}
//	for(int i = 0; i < 120; i ++){
//			XTop_Write_filter_w_c5_14_Words(&doCnn,i*25,(int*)c5_w+(i*16*25+350),25);
//		}
//	for(int i = 0; i < 120; i ++){
//			XTop_Write_filter_w_c5_15_Words(&doCnn,i*25,(int*)c5_w+(i*16*25+375),25);
//		}
//
//
//
//
//
//	XTop_Write_filter_b_c5_Words(&doCnn,0,(int*)c5_b,120);
//
//
//
//	XTop_Write_filter_w_fc_Words(&doCnn,0,(int*)fc_w,1200);
//	XTop_Write_filter_b_fc_Words(&doCnn,0,(int*)fc_b,10);

	XTop_Start(&doCnn);
	XTop_EnableAutoRestart(&doCnn);

	 float correctnum = 0;
	 Xil_DCacheFlushRange((u32)(input),102400*sizeof(float));

	 Xil_DCacheFlushRange((u32)m_dma_buffer_RX,10*sizeof(float));
	 XTime_GetTime(&start);
	 for(int i =0; i<100; i++){




		 XAxiDma_SimpleTransfer(&axiDma,(u32)(input+i*1024),1024*sizeof(float),XAXIDMA_DMA_TO_DEVICE);
		 XAxiDma_SimpleTransfer(&axiDma,(u32)m_dma_buffer_RX,10*sizeof(float),XAXIDMA_DEVICE_TO_DMA);

		 while(XAxiDma_Busy(&axiDma,XAXIDMA_DMA_TO_DEVICE));

		 while(XAxiDma_Busy(&axiDma,XAXIDMA_DEVICE_TO_DMA));
		 Xil_DCacheInvalidateRange((u32)(input+i*1024),1024*sizeof(float));
		 Xil_DCacheInvalidateRange((u32)m_dma_buffer_RX,10*sizeof(float));


		 if(find_max(m_dma_buffer_RX) == label[i])
			 correctnum++;
	 }

	 XTime_GetTime(&end);
	 float correctness = correctnum/100;
	 printf("correction is %f\n",correctness);
	 float hw_time =  1.0 * (end - start) / (COUNTS_PER_SECOND/1000000);
	 printf("HW Output took %.2f us.\n",
			 hw_time);

	 XTime_GetTime(&start);
	printf("software start\n");
	correctnum = 0;
	 for(int i = 0; i < 100; i++){
		 if(label[i] == sw_top(input+i*1024)){
			 correctnum++;
		 }
	 }

	 XTime_GetTime(&end);
	correctness = correctnum/100;
	printf("correction is %f\n",correctness);
	float sw_time =  1.0 * (end - start) / (COUNTS_PER_SECOND/1000000);
	 printf("SW Output took %.2f us.\n",
			 sw_time);
	 printf("Speed up is %f!\n",sw_time/hw_time);
	return 0;
}

float find_max(float* input){
	float max_probability = input[0];
	float max = 0;
	for(int i = 0; i < 10; i++){
		if(input[i] > max_probability ){
			max_probability  = input[i];
			max = (float)i;
		}
	}
	return max;
}

void conv1(float *input,float filter_w_c1[c1_filter*c1_filter_size*c1_filter_size],
		float filter_b_c1[c1_filter],float (*conv1_out)[c1_output_size][c1_output_size])
{
	float temp = 0;
	for(int kernel = 0; kernel < c1_filter; kernel++){
		for(int input_row = 0; input_row < c1_output_size; input_row++){
			for(int input_col = 0; input_col < c1_output_size; input_col++){
				temp = 0;
				for(int kernel_row = 0; kernel_row < c1_filter_size; kernel_row++){

					for(int kernel_col = 0; kernel_col < c1_filter_size; kernel_col++){
					//	#pragma HLS PIPELINE
						temp += filter_w_c1[kernel*c1_filter_size*c1_filter_size+kernel_row*c1_filter_size+kernel_col]* input[(input_row+kernel_row)*32+input_col+kernel_col];
					}
				}
				conv1_out[kernel][input_row][input_col] = _tanh(temp + filter_b_c1[kernel]);
				//printf("%f\n",conv1_out[kernel][input_row][input_col]);
			}
		}
	}
}
void maxpooling2(
		float (*input)[c1_output_size][c1_output_size],float (*output)[s2_output_size][s2_output_size]
)
{
	int stride_row = c1_output_size/s2_stride;
	int stride_col = c1_output_size/s2_stride;
	for(int num = 0; num < c1_output_width; num++){
		for(int row = 0; row < stride_row; row++){
			for(int col = 0; col < stride_col; col++){
				int big_row = row*s2_stride;
				int big_col = col*s2_stride;
				float max = input[num][big_row][big_col];
				for(int row_P = 0; row_P <  s2_pooling_size; row_P ++){
					for(int col_P = 0; col_P <  s2_pooling_size; col_P ++){
						if (input[num][big_row+row_P][big_col+col_P] > max)
							max = input[num][big_row+row_P][big_col+col_P];
					}
				}
				output[num][row][col] = _tanh(max);
				//printf("%f\n",output[num][row][col]);
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
	// output width 16

	for( int ow = 0; ow < c3_output_width; ow++){
		//input width 6
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
							//printf("maxp_out: x %d y %d z %d   xx %d, yy %d, zz %d\n",iw,i+n,j+m,index,n,m);
						}
					}
					output[ow][i][j] += sum;
				}
			}

		}
		for(int k = 0; k< c3_output_size;k++){
			for(int m = 0; m < c3_output_size;m++){
				output[ow][k][m] = _tanh(output[ow][k][m]+bias[ow]);
				//printf("%f\n",output[ow][k][m]);
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
	for(int num = 0; num < c3_output_width; num++){
		for(int row = 0; row < stride_row; row++){
			for(int col = 0; col < stride_col; col++){
				int big_row = row*s4_stride;
				int big_col = col*s4_stride;
				float max = input[num][big_row][big_col];
				for(int row_P = 0; row_P <  s4_pooling_size; row_P ++){
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

float sw_top(float *input){
	float conv1_out[c1_filter][c1_output_size][c1_output_size]={};
	float maxp2_out[s2_output_width][s2_output_size][s2_output_size]={};
	float conv3_out[c3_output_width][c3_output_size][c3_output_size]={};
	float maxp4_out[c3_output_width][s4_output_size][s4_output_size]={};
	float conv5_out[fc_output_width]={};
	float final_layer_out[output_width]={};
	//XTime start,end;
	//XTime_GetTime(&start);
	conv1(input,c1_w,c1_b,conv1_out);
	//XTime_GetTime(&end);
	//printf("conv1 Output took %.2f us.\n",
		 	           //1.0 * (end - start) / (COUNTS_PER_SECOND/1000000));
	//XTime_GetTime(&start);
	maxpooling2(conv1_out,maxp2_out);
	//XTime_GetTime(&end);
	//printf("pooling2 Output took %.2f us.\n",
			 	         //  1.0 * (end - start) / (COUNTS_PER_SECOND/1000000));
	//XTime_GetTime(&start);
	conv3(maxp2_out,c3_w_sw,c3_b,conv3_out);
	//XTime_GetTime(&end);
	//printf("conv3 Output took %.2f us.\n",
			 	         //  1.0 * (end - start) / (COUNTS_PER_SECOND/1000000));
	//XTime_GetTime(&start);
	maxpooling4(conv3_out,maxp4_out);
	//XTime_GetTime(&end);
	//printf("pooling4 Output took %.2f us.\n",
			 	         //  1.0 * (end - start) / (COUNTS_PER_SECOND/1000000));
	//XTime_GetTime(&start);
	conv5(maxp4_out,c5_w_sw,c5_b,conv5_out);
	//XTime_GetTime(&end);
	//printf("conv5 Output took %.2f us.\n",
			 	          // 1.0 * (end - start) / (COUNTS_PER_SECOND/1000000));
	//XTime_GetTime(&start);
	output6(conv5_out,fc_w,fc_b,final_layer_out);
	//XTime_GetTime(&end);
	//printf("outputlayer Output took %.2f us.\n",
			 	          // 1.0 * (end - start) / (COUNTS_PER_SECOND/1000000));

	return find_max(final_layer_out);
}

