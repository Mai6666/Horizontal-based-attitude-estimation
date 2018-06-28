 #include "sub.h"
#include "PE.h"

type_t inter(int rb, int z, int theta, int rst, type_t my_cos, type_t my_sin){
	static type_t step[180];
#pragma HLS ARRAY_PARTITION variable=step complete dim=0
	static type_t row[180];
#pragma HLS ARRAY_PARTITION variable=row complete dim=0
	static type_t col[180];
#pragma HLS ARRAY_PARTITION variable=col complete dim=0

#pragma HLS PIPELINE
	if(!rst){
		col[theta] = 0;
		if(theta < 90){
			row[theta] = 0;
		}
		else {
			row[theta] = my_cos;
		}
	}

	if(!rb){
		col[theta] += step[theta];
	}
	else{
		col[theta] = row[theta];
		row[theta] += N*my_sin;

	}

	step[theta] = (z+1)*M*my_cos;



	return col[theta];

}

int intra(int *vo, type_t f, type_t my_cos, type_t my_sin){
	vo[0] = f+my_sin;
	vo[1] = f+my_cos;
	vo[2] = f+my_sin+my_cos;
	vo[3] = f+2*my_sin;
	vo[4] = f+2*my_sin+my_cos;
	vo[5] = f+3*my_sin;
	vo[6] = f+3*my_sin+my_cos;

	return 0;
}

int vote_c(int *code, int *vo, int *v){
	int i;
	v[0] += code[0];
	for(i=1; i<8; i++)
#pragma HLS UNROLL factor=7
		v[vo[i-1]] += code[i];
	return 0;
}
