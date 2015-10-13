#ifndef _TRANSFORMATION_H_
#define _TRANSFORMATION_H_

#include "matrix.h"
#include "vector.h"

matrix_t identity();
matrix_t translate(float dx, float dy, float dz);
matrix_t scale(float kx, float ky, float kz);
matrix_t rotate_x(float theta);
matrix_t rotate_y(float theta);
matrix_t rotate_z(float theta);
matrix_t rotate(vector_t axis, float theta);
#endif
