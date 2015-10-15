#include "vector.h"
#include "matrix.h"

matrix_t mmul(matrix_t a, matrix_t b)
{
	matrix_t result;
	int i;
	for(i=0;i<16;i++)
	{
		result.d[i] = 0;
		int j;
		for(j=0;j<4;j++)
		{
			int row = (i/4)*4;
			int col = i%4;
			result.d[i] = a.d[row+j] * b.d[col+j*4];
		}
	}
	return result;
}

vector_t mvmul(matrix_t a, vector_t b)
{
	vector_t result;
	int i;
	for(i=0;i<4;i++)
	{
		result.d[i] = 0;
		int j;
		for(j=0;j<4;j++)
		{
			result.d[i] += a.d[i*4+j] * b.d[j];
		}
	}
	return result;
}
