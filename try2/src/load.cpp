/*
 * load.c
 *
 *  Created on: 2019Äê3ÔÂ21ÈÕ
 *      Author: biglo
 */

#include "load.h"
void load_data(char fileName[10],float* weight, int size ){

	xil_printf("File reading start.\n");
	xil_printf("File name is %s.\n", fileName);
	FILE * fp;
	char  line[30];

	int i = 0;
	fp = fopen(fileName, "r");
	if (fp == NULL){
		xil_printf("File dir is null.\n");
		exit(EXIT_FAILURE);
	}
	xil_printf("File reading ok.\n");
	while ( fgets ( line, sizeof(line), fp ) != NULL ) /* read a line */
	{
		if (i == size){
			break;
		}
		//printf("%s\n", line);
		float fff = atof(line);
		weight[i] = fff;
		xil_printf("%f\n", weight[i]);
		i++;
	}

	fclose(fp);

}
