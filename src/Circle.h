#ifndef _LOD_CIRCLE_H_
#define _LOD_CIRCLE_H_
#define GL_GLEXT_PROTOTYPES
#include <glcorearb.h>

class Circle
{
public:
	Circle(int vertexNumber);
	~Circle();
	void render();
private:
	int vertexNumber;
	GLuint buffer;
};
#endif
