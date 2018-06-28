#ifndef SUB_H_
#define SUB_H_

#include <assert.h>
#include <iostream>
#include <stdint.h>
#include <ap_int.h>
#include <hls_stream.h>
#include "ap_fixed.h"
#include <ap_axi_sdata.h>

#define HORIZONTAL_PIXEL_WIDTH 40
#define VERTICAL_PIXEL_HEIGHT 4
#define ALL_PIXEL_VALUE    (HORIZONTAL_PIXEL_HORIZONTAL_PIXEL_WIDTH*VERTICAL_PIXEL_HORIZONTAL_PIXEL_WIDTH)
#define MAX_RHO 800

#define M 2
#define N 4

//typedef uint32_t data_t;
typedef int data_t;
typedef ap_fixed<32,12> type_t;
//typedef double type_t;

// External function prototypes


#endif
