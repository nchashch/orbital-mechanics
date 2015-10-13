#ifndef _MATRIX_H_
#define _MATRIX_H_
#include "vector.h"

typedef struct
{
	float d[16]; /* d[0] d[1] d[2] d[3] is first row */
} matrix_t;

matrix_t mmul(matrix_t a, matrix_t b);
vector_t mvmul(matrix_t a, vector_t b);
#endif
