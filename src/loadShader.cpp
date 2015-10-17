#include <iostream>
#include <fstream>
#include <string>
#define GL_GLEXT_PROTOTYPES
#include <glcorearb.h>

GLuint loadShader(GLenum type, std::string filename)
{
	std::ifstream ifs(filename.c_str());
	std::string shaderSrc = "";
	if(ifs.is_open())
	{
		std::string buf = "";
		while(std::getline(ifs, buf))
		{
			shaderSrc += buf;
			shaderSrc += '\n';
		}
		ifs.close();
	}

	GLint shaderSrcLength;
	GLint infoLogLength;
	char* infoLog = NULL;

	GLuint shaderId = glCreateShader(type);
	shaderSrcLength = shaderSrc.length();
	char const * shaderSrcPtr = shaderSrc.c_str();
	glShaderSource(shaderId, 1, &shaderSrcPtr, &shaderSrcLength);
	glCompileShader(shaderId);

	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
	if(infoLogLength > 0)
	{
		infoLog = new char[infoLogLength + 1];
		infoLog[infoLogLength] = '\0';
		glGetShaderInfoLog(shaderId, infoLogLength, NULL, infoLog);
		std::cout << infoLog;
		delete infoLog;
		infoLog = NULL;
	}
	return shaderId;
}

GLuint loadProgram(GLuint vertexShader, GLuint fragmentShader)
{
	GLuint programId = glCreateProgram();
	glAttachShader(programId, vertexShader);
	glAttachShader(programId, fragmentShader);
	glLinkProgram(programId);

	GLint infoLogLength;
	char* infoLog = NULL;

	glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);
	if(infoLogLength > 0)
	{
		infoLog = new char[infoLogLength + 1];
		infoLog[infoLogLength] = '\0';
		glGetProgramInfoLog(programId, infoLogLength, NULL, infoLog);
		std::cout << infoLog << std::endl;
		delete infoLog;
		infoLog = NULL;
	}
	return programId;
}
