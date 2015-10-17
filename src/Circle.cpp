#include <cmath>
#include "Circle.h"

int intPow(int base, int degree);
void circleData(int numberVertices, GLuint buffer);

void Circle::render(int LOD) {
	if(LOD < 0 || LOD >= LODs_number)
	{
		return;
	}
	int count = intPow(2, LOD_min) * intPow(2, LOD);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[LOD]);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_LINE_LOOP, 0, count);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

int intPow(int base, int degree);
void circleData(int numberVertices, GLuint buffer);

Circle::Circle(int LODs_number, int LOD_min)
{
	buffers = new GLuint[LODs_number];
	glGenBuffers(LODs_number, buffers);
	int LOD = intPow(2, LOD_min);
	for(int i = 0; i < LODs_number; ++i)
	{
		circleData(LOD, buffers[i]);
	}
}

void circleData(int numberVertices, GLuint buffer)
{
	float *vertices = new float[numberVertices * 3];

	const float radiansStep = 2*M_PI / numberVertices;
	for(int i = 0; i < numberVertices; ++i)
	{
		float theta = radiansStep * i;
		vertices[numberVertices + 0] = cos(theta);
		vertices[numberVertices + 1] = sin(theta);
		vertices[numberVertices + 2] = 0.0f;
	}
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, numberVertices * sizeof(float) * 3, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

int intPow(int base, int degree)
{
	int exp = 1;
	for(int i = 0; i < exp; ++i)
	{
		exp *= base;
	}
	return exp;
}

Circle::~Circle()
{
	glDeleteBuffers(LODs_number, buffers);
	delete buffers;
}
