#ifndef _VECTOR_H_
#define _VECTOR_H_
typedef union
{
	float d[4];
	float x, y, z, w;
} vector_t;

vector_t vadd(vector_t a, vector_t b);
vector_t vsub(vector_t a, vector_t b);
vector_t vmul(vector_t a, float b);
vector_t vdiv(vector_t a, float b);
float dot(vector_t a, vector_t b);
vector_t cross(vector_t a, vector_t b);
float length(vector_t a);
vector_t norm(vector_t a);
vector_t neg(vector_t a);
#endif
