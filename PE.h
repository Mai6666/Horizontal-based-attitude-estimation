#ifndef PE_H_
#define PE_H_

#include "sub.h"

type_t inter(int rb, int z, int theta, int rst, type_t my_cos, type_t my_sin);
int intra(int *vo, type_t f, type_t my_cos, type_t my_sin);
int vote_c(int *code, int *vo, int *v);

#endif
