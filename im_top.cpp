#include "sub.h"
#include "fil.h"

int im_top(int x, int pix){
#pragma HLS PIPELINE

	int i, j=0, k=0;
	int pix2, pix3, pix4, pix5, pix6, out=0;

	static unsigned int line_buf[4][HORIZONTAL_PIXEL_WIDTH];
	static unsigned int line_buf2[4][HORIZONTAL_PIXEL_WIDTH];
	static unsigned int line_buf3[4][HORIZONTAL_PIXEL_WIDTH];
	static unsigned int line_buf4[4][HORIZONTAL_PIXEL_WIDTH];
	static unsigned int line_buf5[4][HORIZONTAL_PIXEL_WIDTH];
	static unsigned int line_buf6[4][HORIZONTAL_PIXEL_WIDTH];
	static unsigned int line_buf7[4][HORIZONTAL_PIXEL_WIDTH];
#pragma HLS array_partition variable=line_buf block factor=4 dim=1
#pragma HLS resource variable=line_buf core=RAM_2P
#pragma HLS array_partition variable=line_buf2 block factor=4 dim=1
#pragma HLS resource variable=line_buf2 core=RAM_2P
#pragma HLS array_partition variable=line_buf3 block factor=4 dim=1
#pragma HLS resource variable=line_buf3 core=RAM_2P
#pragma HLS array_partition variable=line_buf4 block factor=4 dim=1
#pragma HLS resource variable=line_buf4 core=RAM_2P
#pragma HLS array_partition variable=line_buf5 block factor=4 dim=1
#pragma HLS resource variable=line_buf5 core=RAM_2P
#pragma HLS array_partition variable=line_buf6 block factor=4 dim=1
#pragma HLS resource variable=line_buf6 core=RAM_2P

	static int pix_mat[5][5]={{0}};
	static int pix_mat2[5][5]={{0}};
	static int pix_mat3[5][5]={{0}};
	static int pix_mat4[5][5]={{0}};
	static int pix_mat5[5][5]={{0}};
	static int pix_mat6[5][5]={{0}};
#pragma HLS array_partition variable=pix_mat complete
#pragma HLS array_partition variable=pix_mat2 complete
#pragma HLS array_partition variable=pix_mat3 complete
#pragma HLS array_partition variable=pix_mat4 complete
#pragma HLS array_partition variable=pix_mat5 complete
#pragma HLS array_partition variable=pix_mat6 complete
#pragma HLS resource variable=pix_mat core=RAM_1P
#pragma HLS resource variable=pix_mat2 core=RAM_1P
#pragma HLS resource variable=pix_mat3 core=RAM_1P
#pragma HLS resource variable=pix_mat4 core=RAM_1P
#pragma HLS resource variable=pix_mat5 core=RAM_1P
#pragma HLS resource variable=pix_mat6 core=RAM_1P

	    int val, a, tmp;

#pragma HLS INLINE region


	ROOP1:for (int k=0; k<5; k++){
		for (int m=0; m<4; m++){
#pragma HLS UNROLL
			pix_mat[k][m] = pix_mat[k][m+1];
		}
	}

	pix_mat[0][4] = line_buf[0][x];
	pix_mat[1][4] = line_buf[1][x];
	pix_mat[2][4] = line_buf[2][x];
	pix_mat[3][4] = line_buf[3][x];
	pix_mat[4][4] = pix;

	line_buf[0][x] = line_buf[1][x];
	line_buf[1][x] = line_buf[2][x]; // 行の入れ替え
	line_buf[2][x] = line_buf[3][x];
	line_buf[3][x] = pix;


	val = 0;
	for(a=16; a>=0; a-=8){
#pragma HLS UNROLL factor=3
		tmp = erode(
				(pix_mat[0][0]>>a)&0xff, (pix_mat[0][1]>>a)&0xff, (pix_mat[0][2]>>a)&0xff, (pix_mat[0][3]>>a)&0xff, (pix_mat[0][4]>>a)&0xff,
				(pix_mat[1][0]>>a)&0xff, (pix_mat[1][1]>>a)&0xff, (pix_mat[1][2]>>a)&0xff, (pix_mat[1][3]>>a)&0xff, (pix_mat[1][4]>>a)&0xff,
				(pix_mat[2][0]>>a)&0xff, (pix_mat[2][1]>>a)&0xff, (pix_mat[2][2]>>a)&0xff, (pix_mat[2][3]>>a)&0xff, (pix_mat[2][4]>>a)&0xff,
				(pix_mat[3][0]>>a)&0xff, (pix_mat[3][1]>>a)&0xff, (pix_mat[3][2]>>a)&0xff, (pix_mat[3][3]>>a)&0xff, (pix_mat[3][4]>>a)&0xff,
				(pix_mat[4][0]>>a)&0xff, (pix_mat[4][1]>>a)&0xff, (pix_mat[4][2]>>a)&0xff, (pix_mat[4][3]>>a)&0xff, (pix_mat[4][4]>>a)&0xff);
		val += tmp << a;
	}

	pix2 = val; // RGB同じ値を入れる
	if (x<4 ) // 最初の2行とその他の行の最初の2列は無効データなので0とする
		pix2 = 0;

	ROOP2:for (int k=0; k<5; k++){
		for (int m=0; m<4; m++){
#pragma HLS UNROLL
			pix_mat2[k][m] = pix_mat2[k][m+1];
		}
	}

	pix_mat2[0][4] = line_buf2[0][x];
	pix_mat2[1][4] = line_buf2[1][x];
	pix_mat2[2][4] = line_buf2[2][x];
	pix_mat2[3][4] = line_buf2[3][x];
	pix_mat2[4][4] = pix2;

	line_buf2[0][x] = line_buf2[1][x];
	line_buf2[1][x] = line_buf2[2][x]; // 行の入れ替え
	line_buf2[2][x] = line_buf2[3][x];
	line_buf2[3][x] = pix2;

	val = 0;
	for(a=16; a>=0; a-=8){
		#pragma HLS UNROLL factor=3
		tmp = dilate(
				(pix_mat2[0][0]>>a)&0xff, (pix_mat2[0][1]>>a)&0xff, (pix_mat2[0][2]>>a)&0xff, (pix_mat2[0][3]>>a)&0xff, (pix_mat2[0][4]>>a)&0xff,
				(pix_mat2[1][0]>>a)&0xff, (pix_mat2[1][1]>>a)&0xff, (pix_mat2[1][2]>>a)&0xff, (pix_mat2[1][3]>>a)&0xff, (pix_mat2[1][4]>>a)&0xff,
				(pix_mat2[2][0]>>a)&0xff, (pix_mat2[2][1]>>a)&0xff, (pix_mat2[2][2]>>a)&0xff, (pix_mat2[2][3]>>a)&0xff, (pix_mat2[2][4]>>a)&0xff,
				(pix_mat2[3][0]>>a)&0xff, (pix_mat2[3][1]>>a)&0xff, (pix_mat2[3][2]>>a)&0xff, (pix_mat2[3][3]>>a)&0xff, (pix_mat2[3][4]>>a)&0xff,
				(pix_mat2[4][0]>>a)&0xff, (pix_mat2[4][1]>>a)&0xff, (pix_mat2[4][2]>>a)&0xff, (pix_mat2[4][3]>>a)&0xff, (pix_mat2[4][4]>>a)&0xff);
		val += tmp << a;
	}

	pix3 = val;
	if(x<4)
		pix3 = 0;

	ROOP3:for (int k=0; k<5; k++){
		for (int m=0; m<4; m++){
#pragma HLS UNROLL
			pix_mat3[k][m] = pix_mat3[k][m+1];
		}
	}

	pix_mat3[0][4] = line_buf3[0][x];
	pix_mat3[1][4] = line_buf3[1][x];
	pix_mat3[2][4] = line_buf3[2][x];
	pix_mat3[3][4] = line_buf3[3][x];
	pix_mat3[4][4] = pix3;

	line_buf3[0][x] = line_buf3[1][x];
	line_buf3[1][x] = line_buf3[2][x]; // 行の入れ替え
	line_buf3[2][x] = line_buf3[3][x];
	line_buf3[3][x] = pix3;

		val = 0;
	for(a=16; a>=0; a-=8){
		#pragma HLS UNROLL factor=3
		tmp = dilate(
				(pix_mat3[0][0]>>a)&0xff, (pix_mat3[0][1]>>a)&0xff, (pix_mat3[0][2]>>a)&0xff, (pix_mat3[0][3]>>a)&0xff, (pix_mat3[0][4]>>a)&0xff,
				(pix_mat3[1][0]>>a)&0xff, (pix_mat3[1][1]>>a)&0xff, (pix_mat3[1][2]>>a)&0xff, (pix_mat3[1][3]>>a)&0xff, (pix_mat3[1][4]>>a)&0xff,
				(pix_mat3[2][0]>>a)&0xff, (pix_mat3[2][1]>>a)&0xff, (pix_mat3[2][2]>>a)&0xff, (pix_mat3[2][3]>>a)&0xff, (pix_mat3[2][4]>>a)&0xff,
				(pix_mat3[3][0]>>a)&0xff, (pix_mat3[3][1]>>a)&0xff, (pix_mat3[3][2]>>a)&0xff, (pix_mat3[3][3]>>a)&0xff, (pix_mat3[3][4]>>a)&0xff,
				(pix_mat3[4][0]>>a)&0xff, (pix_mat3[4][1]>>a)&0xff, (pix_mat3[4][2]>>a)&0xff, (pix_mat3[4][3]>>a)&0xff, (pix_mat3[4][4]>>a)&0xff);
		val += tmp << a;
	}

	pix4 = val;
	if(x<4)
		pix4 = 0;

	ROOP4:for (int k=0; k<5; k++){
		for (int m=0; m<4; m++){
#pragma HLS UNROLL
			pix_mat4[k][m] = pix_mat4[k][m+1];
		}
	}

	pix_mat4[0][4] = line_buf4[0][x];
	pix_mat4[1][4] = line_buf4[1][x];
	pix_mat4[2][4] = line_buf4[2][x];
	pix_mat4[3][4] = line_buf4[3][x];
	pix_mat4[4][4] = pix4;

	line_buf4[0][x] = line_buf4[1][x];
	line_buf4[1][x] = line_buf4[2][x]; // 行の入れ替え
	line_buf4[2][x] = line_buf4[3][x];
	line_buf4[3][x] = pix4;

	val = 0;
	for(a=16; a>=0; a-=8){
		#pragma HLS UNROLL factor=3
		tmp = erode(
				(pix_mat4[0][0]>>a)&0xff, (pix_mat4[0][1]>>a)&0xff, (pix_mat4[0][2]>>a)&0xff, (pix_mat4[0][3]>>a)&0xff, (pix_mat4[0][4]>>a)&0xff,
				(pix_mat4[1][0]>>a)&0xff, (pix_mat4[1][1]>>a)&0xff, (pix_mat4[1][2]>>a)&0xff, (pix_mat4[1][3]>>a)&0xff, (pix_mat4[1][4]>>a)&0xff,
				(pix_mat4[2][0]>>a)&0xff, (pix_mat4[2][1]>>a)&0xff, (pix_mat4[2][2]>>a)&0xff, (pix_mat4[2][3]>>a)&0xff, (pix_mat4[2][4]>>a)&0xff,
				(pix_mat4[3][0]>>a)&0xff, (pix_mat4[3][1]>>a)&0xff, (pix_mat4[3][2]>>a)&0xff, (pix_mat4[3][3]>>a)&0xff, (pix_mat4[3][4]>>a)&0xff,
				(pix_mat4[4][0]>>a)&0xff, (pix_mat4[4][1]>>a)&0xff, (pix_mat4[4][2]>>a)&0xff, (pix_mat4[4][3]>>a)&0xff, (pix_mat4[4][4]>>a)&0xff);
		val += tmp << a;
	}

	pix5 = val;
	if(x<4)
		pix5 = 0;

ROOP5:for (int k=0; k<5; k++){
		for (int m=0; m<4; m++){
#pragma HLS UNROLL
			pix_mat5[k][m] = pix_mat5[k][m+1];
		}
	}

	pix_mat5[0][4] = line_buf5[0][x];
	pix_mat5[1][4] = line_buf5[1][x];
	pix_mat5[2][4] = line_buf5[2][x];
	pix_mat5[3][4] = line_buf5[3][x];
	pix_mat5[4][4] = pix5;

	line_buf5[0][x] = line_buf5[1][x];
	line_buf5[1][x] = line_buf5[2][x]; // 行の入れ替え
	line_buf5[2][x] = line_buf5[3][x];
	line_buf5[3][x] = pix5;

	val = 0;
	for(a=16; a>=0; a-=8){
		#pragma HLS UNROLL factor=3
		tmp = sobel5(
				(pix_mat5[0][0]>>a)&0xff, (pix_mat5[0][1]>>a)&0xff, (pix_mat5[0][2]>>a)&0xff, (pix_mat5[0][3]>>a)&0xff, (pix_mat5[0][4]>>a)&0xff,
				(pix_mat5[2][0]>>a)&0xff, (pix_mat5[2][1]>>a)&0xff, (pix_mat5[2][2]>>a)&0xff, (pix_mat5[2][3]>>a)&0xff, (pix_mat5[2][4]>>a)&0xff,
				(pix_mat5[4][0]>>a)&0xff, (pix_mat5[4][1]>>a)&0xff, (pix_mat5[4][2]>>a)&0xff, (pix_mat5[4][3]>>a)&0xff, (pix_mat5[4][4]>>a)&0xff,
				200);
		val += tmp << a;
	}

	if(x<4)
		pix6 = 0;

	pix6 = val;

	ROOP6:for (int k=0; k<5; k++){
			for (int m=0; m<4; m++){
#pragma HLS UNROLL
				pix_mat6[k][m] = pix_mat6[k][m+1];
			}
		}

	pix_mat6[0][4] = line_buf6[0][x];
	pix_mat6[1][4] = line_buf6[1][x];
	pix_mat6[2][4] = line_buf6[2][x];
	pix_mat6[3][4] = line_buf6[3][x];
	pix_mat6[4][4] = pix6;

	line_buf6[0][x] = line_buf6[1][x];
	line_buf6[1][x] = line_buf6[2][x]; // 行の入れ替え
	line_buf6[2][x] = line_buf6[3][x];
	line_buf6[3][x] = pix6;

	val = 0;
	for(a=16; a>=0; a-=8){
		#pragma HLS UNROLL factor=3
		tmp = 	((pix_mat6[0][0]>>a)&0xff) | ((pix_mat6[0][1]>>a)&0xff) | ((pix_mat6[0][2]>>a)&0xff) | ((pix_mat6[0][3]>>a)&0xff) | ((pix_mat6[0][4]>>a)&0xff) |
				((pix_mat6[1][0]>>a)&0xff) | ((pix_mat6[1][1]>>a)&0xff) | ((pix_mat6[1][2]>>a)&0xff) | ((pix_mat6[1][3]>>a)&0xff) | ((pix_mat6[1][4]>>a)&0xff) |
				((pix_mat6[2][0]>>a)&0xff) | ((pix_mat6[2][1]>>a)&0xff) | ((pix_mat6[2][2]>>a)&0xff) | ((pix_mat6[2][3]>>a)&0xff) | ((pix_mat6[2][4]>>a)&0xff) |
				((pix_mat6[3][0]>>a)&0xff) | ((pix_mat6[3][1]>>a)&0xff) | ((pix_mat6[3][2]>>a)&0xff) | ((pix_mat6[3][3]>>a)&0xff) | ((pix_mat6[3][4]>>a)&0xff) |
				((pix_mat6[4][0]>>a)&0xff) | ((pix_mat6[4][1]>>a)&0xff) | ((pix_mat6[4][2]>>a)&0xff) | ((pix_mat6[4][3]>>a)&0xff) | ((pix_mat6[4][4]>>a)&0xff);
		val += tmp << a;
	}

	out = (val & 0xff) & (val >> 8 & 0xff ) & (val >> 16 & 0xff);

	if(x<4)
		out = 0;

	return out;
}
