#include "fil.h"

int sobel5(
		int x0y0, int x1y0, int x2y0, int x3y0, int x4y0,
		int x0y2, int x1y2, int x2y2, int x3y2, int x4y2,
		int x0y4, int x1y4, int x2y4, int x3y4, int x4y4,
		int th){
	int y;

	y = (x0y0 + 4*x1y0 + 6*x2y0 + 4*x3y0 + x4y0
		 -2*x0y2 -8*x1y2 -12*x2y2 -8*x3y2 -2*x4y2
		 + x0y4 + 4*x1y4 + 6*x2y4 + 4*x3y4 + x4y4);

	if(y<0) y = -1*y;
	if (y>th)
		y = 1;
	else
		y = 0;

	return y;
}

int dilate(
		int x0y0, int x1y0, int x2y0, int x3y0, int x4y0,
		int x0y1, int x1y1, int x2y1, int x3y1, int x4y1,
		int x0y2, int x1y2, int x2y2, int x3y2, int x4y2,
		int x0y3, int x1y3, int x2y3, int x3y3, int x4y3,
		int x0y4, int x1y4, int x2y4, int x3y4, int x4y4){
	int max;
	max = x2y0;
	if(x0y1 > max) max = x0y1;
	if(x1y1 > max) max = x1y1;
	if(x2y1 > max) max = x2y1;
	if(x3y1 > max) max = x3y1;
	if(x4y1 > max) max = x4y1;
	if(x0y2 > max) max = x0y2;
	if(x1y2 > max) max = x1y2;
	if(x2y2 > max) max = x2y2;
	if(x3y2 > max) max = x3y2;
	if(x4y2 > max) max = x4y2;
	if(x0y3 > max) max = x0y3;
	if(x1y3 > max) max = x1y3;
	if(x2y3 > max) max = x2y3;
	if(x3y3 > max) max = x3y3;
	if(x4y3 > max) max = x4y3;
	if(x2y4 > max) max = x2y4;
	//printf("%d\n",max);
	return (max);
}

int erode(
		int x0y0, int x1y0, int x2y0, int x3y0, int x4y0,
		int x0y1, int x1y1, int x2y1, int x3y1, int x4y1,
		int x0y2, int x1y2, int x2y2, int x3y2, int x4y2,
		int x0y3, int x1y3, int x2y3, int x3y3, int x4y3,
		int x0y4, int x1y4, int x2y4, int x3y4, int x4y4){
	int min;
	min = x2y0;

	if(x0y1 < min) min = x0y1;
	if(x1y1 < min) min = x1y1;
	if(x2y1 < min) min = x2y1;
	if(x3y1 < min) min = x3y1;
	if(x4y1 < min) min = x4y1;
	if(x0y2 < min) min = x0y2;
	if(x1y2 < min) min = x1y2;
	if(x2y2 < min) min = x2y2;
	if(x3y2 < min) min = x3y2;
	if(x4y2 < min) min = x4y2;
	if(x0y3 < min) min = x0y3;
	if(x1y3 < min) min = x1y3;
	if(x2y3 < min) min = x2y3;
	if(x3y3 < min) min = x3y3;
	if(x4y3 < min) min = x4y3;
	if(x2y4 < min) min = x2y4;
	//printf("%d\n",min);
	return (min);
}
