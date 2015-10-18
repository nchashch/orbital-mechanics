#ifndef _LOD_CIRCLE_H_
#define _LOD_CIRCLE_H_
#define GL_GLEXT_PROTOTYPES
#include <glcorearb.h>
#include <glm.hpp>

class Circle
{
public:
	Circle(int vertexNumber);
	~Circle();
	void render
		(GLuint program,
		glm::mat4 modelView, glm::mat4 perspective,
		glm::vec4 color, GLenum mode) const;
private:
	int vertexNumber;
	GLuint buffer;
};
#endif
