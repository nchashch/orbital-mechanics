#ifndef _RENDERER_H_
#define _RENDERER_H_
#define GL_GLEXT_PROTOTYPES
#include <glcorearb.h>
#include <glm.hpp>
#include "Circle.h"
#include "Mesh.h"
#include "Object.h"
class Renderer
{
public:
	Renderer
		(std::string vertexShaderFilename,
		std::string fragmentShaderFilename,
		std::string objectFilename,
		std::string earthFilename,
	 	double FOV, double aspect,
	 	double near, double far);
	~Renderer();
	void frameTick(float theta, float phi, float R);
	void clear();
	void render(Object object);
	void renderEarth();
private:
	Circle circle;
	Mesh earth;
	Mesh object;
	glm::mat4 projection;
	glm::mat4 camera;
	glm::vec4 colorObject;
	glm::vec4 colorOrbit;
	GLuint program;
	GLuint vertexShader;
	GLuint fragmentShader;
	float theta;
	float phi;
	float R;
};
#endif
