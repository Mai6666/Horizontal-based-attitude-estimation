//bmp_header.h
//2015/07/17 by Masaki Ono

//#pragma once

#include <stdio.h>
//#include <tchar.h>

//BITMAPFILEHEADER 14bytes
typedef struct tagBITMPAFILEHEADER{
	unsigned short bfType;
	unsigned long bfSize;
	unsigned short bfReserved1;
	unsigned short bfReserved2;
	unsigned long bfOffBits;
}BITMAPFILEHEADER;

//BITMAPINFOHEADER 40bytes
typedef struct tagBITMAPINFOHEADER{
	unsigned long biSize;
	long biWidth;
	long biHeight;
	unsigned short biPlanes;
	unsigned short biBitCount;
	unsigned long biCompression;
	unsigned long biSizeImage;
	long biXPixPerMeter;
	long biYPixPerMeter;
	unsigned long biClrUsed;
	unsigned long biClrImportant;
} BITMAPINFOHEADER;

typedef struct BMP24bitsFORMAT{
	unsigned char blue;
	unsigned char green;
	unsigned char red;
}BMP24FORMAT;

