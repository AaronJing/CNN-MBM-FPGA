/*
 * pool.h
 *
 *  Created on: 2019Äê3ÔÂ20ÈÕ
 *      Author: biglo
 */

#ifndef SRC_POOL_H_
#define SRC_POOL_H_
#include "values.h"

void POOL_LAYER_1(float* input, float* kernel, float* bias, float* output, int scale=2);
void POOL_LAYER_2(float* input, float* kernel, float* bias, float* output, int scale=2);

#endif /* SRC_POOL_H_ */
