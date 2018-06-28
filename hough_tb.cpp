#include <iostream>
#include "bmp_header.h"
//#include <math.h>
#include <hls_stream.h>
#include "hls_math.h"
#include "ap_fixed.h"
#include <ap_axi_sdata.h>
#include <time.h>

typedef ap_fixed<32,12> type_t;
typedef int data_t;

int hough(hls::stream<int>& , volatile int *, hls::stream<type_t> &, hls::stream<type_t> &);
int hough_class(hls::stream<int>& , volatile int *, hls::stream<type_t> &, hls::stream<type_t> &);

#define MAX_RHO 800

int main()
{

    hls::stream<data_t> ins("ins");

    int *s, *h;
    long x, y;
    BITMAPFILEHEADER bmpfhr;
    BITMAPINFOHEADER bmpihr;
    FILE *fbmpr, *fbmpw;
    int *rd_bmp, *hw_bmp;

    int blue, green, red;
    char blue_c, green_c, red_c;

    int i, j;
    int vote[180*MAX_RHO*2];
    hls::stream<type_t> co("cos"), si("sin");
    type_t rad = 0.0, theta=180.0, PI=3.14;


if ((fbmpr = fopen("opencountry_land298.bmp", "rb")) == NULL){
        fprintf(stderr, "Can't open test.bmp by binary read mode\n");
        exit(1);
    }
    fread(&bmpfhr.bfType, sizeof(char), 2, fbmpr);
    fread(&bmpfhr.bfSize, sizeof(long), 1, fbmpr);
    fread(&bmpfhr.bfReserved1, sizeof(short), 1, fbmpr);
    fread(&bmpfhr.bfReserved2, sizeof(short), 1, fbmpr);
    fread(&bmpfhr.bfOffBits, sizeof(long), 1, fbmpr);
    fread(&bmpihr, sizeof(BITMAPINFOHEADER), 1, fbmpr);

    if ((rd_bmp =(int *)malloc(sizeof(int) * (bmpihr.biWidth * bmpihr.biHeight))) == NULL){
        fprintf(stderr, "Can't allocate rd_bmp memory\n");
        exit(1);
    }
    if ((hw_bmp =(int *)malloc(sizeof(int) * (bmpihr.biWidth * bmpihr.biHeight))) == NULL){
        fprintf(stderr, "Can't allocate hw_bmp memory\n");
        exit(1);
    }

    for (y=0; y<bmpihr.biHeight; y++){
        for (x=0; x<bmpihr.biWidth; x++){
            blue = fgetc(fbmpr);
            green = fgetc(fbmpr);
            red = fgetc(fbmpr);
            rd_bmp[((bmpihr.biHeight-1)-y)*bmpihr.biWidth+x] = (blue & 0xff) | ((green & 0xff)<<8) | ((red & 0xff)<<16);
            //if(blue != 0) printf("%ld %ld\n",(bmpihr.biHeight-1)-y,x);
            //if( (blue & 0xff) == 0xff) rd_bmp[((bmpihr.biHeight-1)-y)*bmpihr.biWidth+x] = 1;
            //else rd_bmp[((bmpihr.biHeight-1)-y)*bmpihr.biWidth+x] = 0;
        }
    }
    fclose(fbmpr);

    data_t pix;
    for(int j=0; j < bmpihr.biHeight; j++){
        for(i=0; i < bmpihr.biWidth; i++){
            pix = (ap_int<32>)rd_bmp[(j*bmpihr.biWidth)+i];
            ins << pix;
        }
    }

	for(i=0; i<180; i++){
	 for(j=0; j<MAX_RHO; j++)
		 vote[i*MAX_RHO+j] = 0;
	}
	for(i=0; i<180; i++){
		rad = i*PI/theta;
		co << cos(rad);
		si << sin(rad);
	}

    //im_top(ins, (int *)vote, co, si);
	clock_t st, en;

	st = clock();
	hough(ins, (int *)vote, co, si);
	en = clock();
	std::cout << "duration = " << (double)(en - st) / CLOCKS_PER_SEC << "sec.\n";

	int t=0, max=0, rho=0;
	for(theta=0; theta<180; theta++){
		for(int r=0; r<MAX_RHO; r++){
			if(vote[theta*MAX_RHO+r] > max){
				max = vote[theta*MAX_RHO+r];
				t = theta;
				rho = r;
			}
		}
	}

	std::cout << "theta is " << std::dec << t << " rho is " << rho << std::endl;

    if ((fbmpw=fopen("test_result.bmp", "wb")) == NULL){
        fprintf(stderr, "Can't open temp_lap.bmp by binary write mode\n");
        exit(1);
    }
    // BMP驛｢譎�ｽｼ譁撰ｼ憺Δ�ｽｧ�ｿｽ�ｽｽ�ｽ､驛｢譎｢�ｽｽ�ｽｫ驛｢譎擾ｿｽ�ｽ･郢晢ｽ｣驛｢譎｢�ｽｿ�ｽｽ驍ｵ�ｽｺ�ｿｽ�ｽｽ�ｽｮ髫ｴ蜴�ｽｽ�ｽｸ驍ｵ�ｽｺ陝�雜｣�ｽｽ�ｽｾ�ｿｽ�ｽｽ�ｽｼ驍ｵ�ｽｺ�ｿｽ�ｽｽ�ｽｿ
    fwrite(&bmpfhr.bfType, sizeof(char), 2, fbmpw);
    fwrite(&bmpfhr.bfSize, sizeof(long), 1, fbmpw);
    fwrite(&bmpfhr.bfReserved1, sizeof(short), 1, fbmpw);
    fwrite(&bmpfhr.bfReserved2, sizeof(short), 1, fbmpw);
    fwrite(&bmpfhr.bfOffBits, sizeof(long), 1, fbmpw);
    fwrite(&bmpihr, sizeof(BITMAPINFOHEADER), 1, fbmpw);

    // RGB 驛｢譎｢�ｽｿ�ｽｽ驛｢譎｢�ｽｽ�ｽｼ驛｢�ｽｧ�ｿｽ�ｽｽ�ｽｿ驍ｵ�ｽｺ�ｿｽ�ｽｽ�ｽｮ髫ｴ蜴�ｽｽ�ｽｸ驍ｵ�ｽｺ陝�雜｣�ｽｽ�ｽｾ�ｿｽ�ｽｽ�ｽｼ驍ｵ�ｽｺ�ｿｽ�ｽｽ�ｽｿ驍ｵ�ｽｲ�ｿｽ�ｽｿ�ｽｽ鬯ｨ�ｽｾ�ｿｽ�ｽｿ�ｽｽ鬯ｯ�ｿｽ�ｿｽ�ｽｿ�ｽｽ驍ｵ�ｽｺ�ｿｽ�ｽｽ�ｽｫ驍ｵ�ｽｺ陷ｷ�ｽｶ�ｿｽ�ｽｽ�ｿｽ
    for (y=0; y<bmpihr.biHeight; y++){
        for (x=0; x<bmpihr.biWidth; x++){
            blue = rd_bmp[((bmpihr.biHeight-1)-y)*bmpihr.biWidth+x] & 0xff;
            green = (rd_bmp[((bmpihr.biHeight-1)-y)*bmpihr.biWidth+x] >> 8) & 0xff;
            red = (rd_bmp[((bmpihr.biHeight-1)-y)*bmpihr.biWidth+x]>>16) & 0xff;

            fputc(blue, fbmpw);
            fputc(blue, fbmpw);
            fputc(blue, fbmpw);
            //fputc(green, fbmpw);
            //fputc(red, fbmpw);
        }
    }

    fclose(fbmpw);
    free(rd_bmp);
    free(hw_bmp);


    return(0);
}
