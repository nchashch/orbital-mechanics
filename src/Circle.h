#ifndef _LOD_CIRCLE_H_
#define _LOD_CIRCLE_H_
#define GL_GLEXT_PROTOTYPES
#include <glcorearb.h>

class Circle
{
public:
	Circle(int LODs_number, int LOD_min);
	~Circle();
	void render(int LOD);
private:
	int LODs_number;
	int LOD_min; /* Minimum LOD, There are 2^LOD_min vertices in buffers[0] LOD. */
	GLuint *buffers;
};
#endif
