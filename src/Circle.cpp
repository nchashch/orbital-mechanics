#include <cmath>
#include <iostream>
#include "Circle.h"

void Circle::render() {
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glDrawArrays(GL_LINE_LOOP, 0, vertexNumber);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Circle::Circle(int vertexNumber) :
	vertexNumber(vertexNumber)
{
	glGenBuffers(1, &buffer);

	float *vertices = new float[vertexNumber * 3];

	const float radiansStep = 2*M_PI / vertexNumber;
	for(int i = 0; i < vertexNumber; ++i)
	{
		float theta = radiansStep * i;
		vertices[3*i + 0] = cos(theta);
		vertices[3*i + 1] = sin(theta);
		vertices[3*i + 2] = 0.0f;
	}
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, vertexNumber * sizeof(float) * 3, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete vertices;
}

Circle::~Circle()
{
	glDeleteBuffers(1, &buffer);
}
