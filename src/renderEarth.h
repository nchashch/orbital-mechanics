#ifndef _RENDER_EARTH_H_
#define _RENDER_EARTH_H_
#define GL_GLEXT_PROTOTYPES
#include <glm.hpp>
#include <glcorearb.h>
#include "Circle.h"
void renderEarth
		(GLuint program,
		glm::mat4 camera, glm::mat4 projection,
		const Circle &circle,
	       	float phi, float theta);
#endif
