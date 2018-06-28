#include "sub.h"

int rle3(int pix, hls::stream<data_t> &que)
{
#pragma HLS INTERFACE axis port=&que
	int code=0, B=0, zl=0, rb=1, symbol;
	int z=0;

	static int line_buf[4][HORIZONTAL_PIXEL_WIDTH];
#pragma HLS array_partition variable=line_buf block factor=4 dim=1
#pragma HLS resource variable=line_buf core=RAM_2P
	static int m=0, n=0;

#pragma HLS PIPELINE
	if(n > HORIZONTAL_PIXEL_WIDTH-1){
		m++;
		n = 0;
	}
	line_buf[m][n++] = pix;

	if(m==3 && n==HORIZONTAL_PIXEL_WIDTH){
		rb = 1;
		code = (line_buf[0][0]<<7) + (line_buf[0][1]<<6) +
				(line_buf[1][0]<<5) + (line_buf[1][1]<<4) +
				(line_buf[2][0]<<3) + (line_buf[2][1]<<2) +
				(line_buf[3][0]<<1) + (line_buf[3][1]);

		RLE:for(int j=2; j<HORIZONTAL_PIXEL_WIDTH-1; j+=2){
				B = (line_buf[0][j]<<7) + (line_buf[0][j+1]<<6) +
					(line_buf[1][j]<<5) + (line_buf[1][j+1]<<4) +
					(line_buf[2][j]<<3) + (line_buf[2][j+1]<<2) +
					(line_buf[3][j]<<1) + (line_buf[3][j+1]);

//				std::cout << "B " << B << std::endl;
				if(B != 0 || zl > 14){
					symbol = ((rb << 12) + (code << 4) + zl);
//					std::cout << std::hex << std::setw(4) << std::setfill('0') << symbol << std::endl;
					que << symbol;
					rb = 0;
					zl = 0;
					code = B;
				}
				else
					zl += 1;
			}
		symbol = ((rb << 12) + (code << 4) + zl);
//		std::cout << std::hex << std::setw(4) << std::setfill('0') << symbol << std::endl;
		que << symbol;

		m=0;
		n=0;
	}
	return 0;
}
