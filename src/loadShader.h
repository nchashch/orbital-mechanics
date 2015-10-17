#ifndef _LOAD_SHADER_H_
#define _LOAD_SHADER_H_
#define GL_GLEXT_PROTOTYPES
#include <glcorearb.h>

GLuint loadShader(GLenum type, std::string filename);
GLuint loadProgram(GLuint vertexShader, GLuint fragmentShader);
#endif
