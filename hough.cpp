#include "sub.h"
#include "PE.h"

int im_top(int, int);
int rle3(int , hls::stream<data_t>&);
//hls::stream& rle3(unsigned int *);

int hough(hls::stream<int>& ins, volatile int *vote, hls::stream<type_t> &co, hls::stream<type_t> &si){
#pragma HLS INTERFACE axis port=&ins
#pragma HLS INTERFACE s_axilite port=return
#pragma HLS INTERFACE m_axi depth=144000 port=vote offset=slave bundle=vote
#pragma HLS INTERFACE axis port=&si
#pragma HLS INTERFACE axis port=&co

	int pix, out, i=0;
	hls::stream<data_t> que("que");
#pragma HLS STREAM variable=que depth=2560 dim=1

    int rb, c, zl, rst, symbol, tmp[4][40]={
    		{0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 1,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 1,0, 0,0, 1,0},
			{0,0, 0,0, 0,0, 0,0, 0,0, 1,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0},
			{0,0, 0,0, 0,0, 0,0, 1,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0},
			{0,0, 0,0, 0,0, 1,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0}};
    type_t i_f;
    int code[8]={0}, vo[7]={0}, v[5]={0}, svote[5]={0};
#pragma HLS array_partition variable=code complete dim=0
#pragma HLS array_partition variable=vo complete dim=0
#pragma HLS array_partition variable=v complete dim=0
#pragma HLS array_partition variable=svote complete dim=0

    type_t my_cos[180], my_sin[180];
#pragma HLS array_partition variable=my_cos complete dim=0
#pragma HLS array_partition variable=my_sin complete dim=0

#pragma HLS INLINE region

	Initialize:for(int theta=0; theta<180; theta++){
#pragma HLS PIPELINE
		co >> my_cos[theta];
		si >> my_sin[theta];
	}

	rst = 0;
	int theta=0;
	YROOP:for(int y=0; y<VERTICAL_PIXEL_HEIGHT; y++){
		XROOP:for(int x=0; x<HORIZONTAL_PIXEL_WIDTH; x++){
#pragma HLS PIPELINE
//			ins >> pix;
//			out = im_top(x, pix);
			out = tmp[y][x];

//			std::cout << "out " << out << std::endl;
			rle3(out, que);
		}
//
//
			while(!que.empty()){
#pragma HLS LOOP_TRIPCOUNT min=0 max=320
				que >> symbol;
				rb = (symbol>>12)&0x01;
				c = (symbol>>4)&0xff;
				zl = symbol&0xf;
				std::cout << std::hex << std::setw(4) << std::setfill('0') << symbol << std::endl;
//				std::cout << rb << " " << c << " " << zl << std::endl;
				HOUGH:for(int theta=0; theta<180; theta++){
#pragma HLS PIPELINE
					if(theta < 90){
						CODE:for(int i=0; i<8; i++)
		#pragma HLS UNROLL factor=8
							code[7-i] = (c>>i)&0x01;
					}
					else{
						for(int i=0; i<8; i+=2){
		#pragma HLS UNROLL factor=4
							code[i] = (c>>(6-i))&0x01;
							code[i+1] = (c>>(7-i))&0x01;
						}
					}

					i_f = inter(rb, zl, theta, rst, my_cos[theta], my_sin[theta]);
					std::cout << std::dec << theta << " " << i_f << std::endl;
					intra(vo, i_f-(int)i_f, my_cos[theta], my_sin[theta]);
					vote_c(code, vo, v);

					if(i_f < 0)
						i_f = -1*i_f;
					memcpy(&svote[0], (const int *)&vote[theta*MAX_RHO+(int)i_f], 5*sizeof(int));
					for(int i=0; i<5; i++){
						svote[i] += v[i];
					}
					memcpy((int *)&vote[theta*MAX_RHO+(int)i_f], (const int *)&svote[0], 5*sizeof(int));

					rst = 1;
				}
			}
	}

	return 0;
}
