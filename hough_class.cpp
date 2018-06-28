#include "sub.h"
#include "PE2.h"

int im_top(int, int);
int rle3(int , hls::stream<data_t>&);

int hough_class(hls::stream<data_t>& in, volatile int * vote, hls::stream<type_t>& co, hls::stream<type_t>& si){
#pragma HLS INTERFACE axis port=&ins
#pragma HLS INTERFACE s_axilite port=return
#pragma HLS INTERFACE m_axi depth=144000 port=vote offset=slave bundle=vote
#pragma HLS INTERFACE axis port=&si
#pragma HLS INTERFACE axis port=&co

	int pix, out, i=0, max=0, max_t=0, max_r=0;
	hls::stream<data_t> que("que");
#pragma HLS STREAM variable=que depth=2560 dim=1

    int rb, c, zl, rst, symbol, tmp;
    type_t i_f;
    int code[8]={0}, vo[7]={0}, v[5]={0}, svote[5]={0};
#pragma HLS array_partition variable=code complete dim=0
#pragma HLS array_partition variable=vo complete dim=0
#pragma HLS array_partition variable=v complete dim=0
#pragma HLS array_partition variable=svote complete dim=0

    type_t my_cos, my_sin;
#pragma HLS array_partition variable=my_cos complete dim=0
#pragma HLS array_partition variable=my_sin complete dim=0

    PrEl PE[180];

#pragma HLS INLINE region

	rst = 0;

	Initialize:for(int theta=0; theta<180; theta++){
#pragma HLS PIPELINE
		co >> my_cos;
		si >> my_sin;
		PE[theta].set(theta, my_cos, my_sin);
	}


	YROOP:for(int y=0; y<VERTICAL_PIXEL_HEIGHT; y++){
		XROOP:for(int x=0; x<HORIZONTAL_PIXEL_WIDTH; x++){
#pragma HLS PIPELINE
			in >> pix;
			out = im_top(x, pix);
			rle3(out, que);
		}

		WHILE:while(!que.empty()){
#pragma HLS LOOP_TRIPCOUNT min=0 max=320
				que >> symbol;
				rb = (symbol>>12)&0x01;
				c = (symbol>>4)&0xff;
				zl = symbol&0xf;

//			std::cout << rb << " " << c << " " << zl << std::endl;

			THETA:for(int theta=0; theta<180; theta++){
	#pragma HLS PIPELINE
				if(theta < 90){
					for(i=0; i<8; i++)
						code[7-i] = (c>>i)&0x01;
				}
				else{
					for(i=0; i<8; i+=2){
						code[i] = (c>>(6-i))&0x01;
						code[i+1] = (c>>(7-i))&0x01;
					}
				}

				//std::cout << "reset" <<std::endl;
				PE[theta].reset();
				//std::cout << "inter" << std::endl;
				PE[theta].inter(rb, zl);
				//std::cout << "intra" << std::endl;
				PE[theta].intra();
				//std::cout << "vote consolidation" << std::endl;
				PE[theta].vote_c(code);
				//std::cout << "vote" << std::endl;

//				if(PE[theta].col < 0)
//					tmp = -1*PE[theta].col;
//				else
//					tmp = PE[theta].col;
				tmp = PE[theta].col+MAX_RHO;

				//std::cout << tmp << std::endl;
				SVOTE_INIT:for(i=0; i<5; i++)
	#pragma HLS UNROLL factor=5
					svote[i] = 0;

				memcpy(&svote[0], (const int*)&vote[theta*MAX_RHO+tmp], 5*sizeof(int));
				VOTE:for(i=0; i<5; i++){
	#pragma HLS UNROLL factor=5
					svote[i] += PE[theta].v[i];
					if(svote[i] > max) {
						max = svote[i];
						max_t = theta;
						max_r = tmp+i;
					}
				}

				memcpy((int*)&vote[theta*MAX_RHO+tmp], (const int*)&svote[0], 5*sizeof(int));

			}
		}
	}
	std::cout << "max rho is " << max_r << " max theta is " << max_t << std::endl;

	return 0;
}
