#include <math.h>
#include "vector.h"

vector_t vadd(vector_t a, vector_t b)
{
	vector_t result;
	int i;
	for(i=0;i<4;i++)
	{
		result.d[i] = a.d[i] + b.d[i];
	}
	return result;
}

vector_t vsub(vector_t a, vector_t b)
{
	return vadd(a, neg(b));
}

vector_t vmul(vector_t a, float b)
{
	vector_t result;
	int i;
	for(i=0;i<4;i++)
	{
		result.d[i] = a.d[i] * b;
	}
	return result;
}

vector_t vdiv(vector_t a, float b)
{
	return vmul(a, 1/b);
}

float dot(vector_t a, vector_t b)
{
	float result = 0;
	int i;
	for(i=0;i<4;i++)
	{
		result += a.d[i] * b.d[i];
	}
	return result;
}

vector_t cross(vector_t a, vector_t b)
{
	vector_t result;
	result.x = a.y*b.z - a.z*b.y;
	result.y = a.z*b.x - a.x*b.z;
	result.z = a.x*b.y - a.y*b.x;
	result.w = 0;
	return result;
}

float length(vector_t a)
{
	return sqrt(dot(a, a));
}

vector_t norm(vector_t a)
{
	return vdiv(a, length(a));
}

vector_t neg(vector_t a)
{
	return vmul(a, -1);
}
