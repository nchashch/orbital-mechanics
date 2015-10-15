#include <math.h>
#include "matrix.h"
#include "vector.h"

matrix_t identity()
{
	matrix_t result = {
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1 };
	return result;
}

matrix_t translate(float dx, float dy, float dz)
{
	matrix_t result = {
	1, 0, 0, dx,
	0, 1, 0, dy,
	0, 0, 1, dz,
	0, 0, 0, 1 };
	return result;
}

matrix_t scale(float kx, float ky, float kz)
{
	matrix_t result = {
	kx,  0,  0, 0,
	 0, ky,  0, 0,
	 0,  0, kz, 0,
	 0,  0,  0, 1 };
	return result;
}

/* https://en.wikipedia.org/wiki/Rotation_matrix */
matrix_t rotate_x(float theta)
{
	float t = theta;
	matrix_t result = {
	1,      0,       0, 0,
	0, cos(t), -sin(t), 0,
	0, sin(t),  cos(t), 0,
	0,      0,       0, 1 };
	return result;
}

matrix_t rotate_y(float theta)
{
	float t = theta;
	matrix_t result = {
	 cos(t), 0, sin(t), 0,
	      0, 1,      0, 0,
	-sin(t), 0, cos(t), 0,
	      0, 0,      0, 1 };
	return result;
}

matrix_t rotate_z(float theta)
{
	float t = theta;
	matrix_t result = {
	cos(t), -sin(t), 0, 0,
	sin(t),  cos(t), 0, 0,
	0,       0,      1, 0,
	0,       0,      0, 1 };
	return result;
}

matrix_t rotate(vector_t axis, float theta)
{
	vector_t a = norm(axis);
	float t = theta;
	float ax = a.x;
	float ay = a.y;
	float az = a.z;
	matrix_t result = {
	   cos(t)+ax*ax*(1-cos(t)), ax*ay*(1-cos(t))-az*sin(t), ax*az*(1-cos(t))+ay*sin(t), 0,
	ay*ax*(1-cos(t))+az*sin(t),    cos(t)+ay*ay*(1-cos(t)), ay*az*(1-cos(t))-ax*sin(t), 0,
	az*ax*(1-cos(t))-ay*sin(t), az*ay*(1-cos(t))+ax*sin(t),    cos(t)+az*az*(1-cos(t)), 0,
				 0,                          0,                          0, 1 };
	return result;
}

matrix_t projection(double fov, double aspect, double near, double far)
{
	double D2R = M_PI / 180.0;
	double yScale = 1.0 / tan(D2R * fov / 2);
	double xScale = yScale / aspect;
	double nearmfar = near - far;
	matrix_t result = {
	xScale,      0,                       0,  0,
	     0, yScale,                       0,  0,
	     0,      0, (far + near) / nearmfar, -1,
	     0,      0, 2*far*near / nearmfar,    0 };
	return result;
}
