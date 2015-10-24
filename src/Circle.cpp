#include <cmath>
#include <iostream>
#include "Circle.h"

void Circle::render
		(GLuint program,
		glm::mat4 modelView, glm::mat4 projection,
		glm::vec4 color, GLenum mode) const
{
	glUseProgram(program);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
	glUniformMatrix4fv(0, 1, GL_FALSE, &modelView[0][0]);
	glUniformMatrix4fv(1, 1, GL_FALSE, &projection[0][0]);
	glUniform4fv(2, 1, &color[0]);
	glDrawArrays(mode, 0, vertexNumber);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);
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
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete[] vertices;
}

Circle::~Circle()
{
	glDeleteBuffers(1, &buffer);
}
