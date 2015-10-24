#ifndef _MESH_H_
#define _MESH_H_
#include <glm.hpp>
#define GL_GLEXT_PROTOTYPES
#include <glcorearb.h>
class Mesh
{
public:
	Mesh(const char* filename);
	~Mesh();
	void render
		(GLuint program,
		 glm::mat4 modelView, glm::mat4 projection,
		 glm::vec4 color, GLenum mode) const;
private:
	GLuint vbuffer;
	GLuint ibuffer;
	int count;
	size_t positions_size;
};
#endif
