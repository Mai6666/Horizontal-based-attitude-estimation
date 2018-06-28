#include "sub.h"

class PrEl{
private:
	int theta;
	int vo[7];
	type_t my_cos, my_sin;
	type_t row, step;

public:
	int  v[5];
	type_t col;

	void set(int, type_t, type_t);
	void reset();
	void inter(int, int);
	void intra();
	void vote_c(int *);
};

void PrEl::set(int th, type_t co, type_t si){
	theta = th;
	my_cos = co;
	my_sin = si;
	step = 0.0;

	col = 0.0;
	if(theta < 90){
		row = 0;
	}
	else {
		row = my_cos;
	}

}

void PrEl::reset(){
	int i;
	for(i=0; i<5; i++){
		vo[i] = 0;
		v[i] = 0;
	}
	vo[5] = 0;
	vo[6] = 0;
}
void PrEl::inter(int rb, int z){

	if(!rb){
		col += step;
		//if(theta == 0) std::cout << "step      " << step << std::endl;
	}
	else{
		col = row;
		//if(theta == 0) std::cout << col << std::endl;
		row += N*my_sin;

	}

	step = (z+1)*M*my_cos;

}

void PrEl::intra(){
	type_t f;

	f = col - (int)col;

	vo[0] = f+my_sin;
	vo[1] = f+my_cos;
	vo[2] = f+my_sin+my_cos;
	vo[3] = f+2*my_sin;
	vo[4] = f+2*my_sin+my_cos;
	vo[5] = f+3*my_sin;
	vo[6] = f+3*my_sin+my_cos;

//	for(int tp=0; tp<7; tp++)
//		std::cout << vo[tp] << std::endl;
}

void PrEl::vote_c(int *code){
	int i;
	v[0] += code[0];
	for(i=1; i<8; i++)
#pragma HLS UNROLL factor=5
		v[vo[i-1]] += code[i];

}

