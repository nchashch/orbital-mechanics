#include <gtc/matrix_transform.hpp>
#include "constants.h"
#include "Renderer.h"
#include "loadShader.h"

Renderer::Renderer
		(std::string vertexShaderFilename,
		std::string fragmentShaderFilename,
		std::string objectFilename,
		std::string earthFilename,
	 	double FOV, double aspect,
	 	double near, double far) :
		object(objectFilename.c_str()),
		circle(64),
		earth(earthFilename.c_str()),
		colorObject(1.0f, 1.0f, 1.0f, 1.0f),
		colorOrbit(1.0f, 1.0f, 1.0f, 1.0f),
		camera(1.0f)
{
	projection = glm::perspective(FOV, aspect, near, far);
	vertexShader = loadShader(GL_VERTEX_SHADER, vertexShaderFilename);
	fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentShaderFilename);
	program = loadProgram(vertexShader, fragmentShader);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glLineWidth(2.0f);
}

Renderer::~Renderer()
{
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteProgram(program);
}

void Renderer::frameTick(float theta, float phi, float R)
{
	float x, y, z;
	x = R * sin(phi) * cos(theta);
	y = R * sin(phi) * sin(theta);
	z = R * cos(phi);
	glm::vec3 eye(x, y, z);
	glm::vec3 center(0.0f, 0.0f, 0.0f);
	glm::vec3 up(0.0f, 0.0f, 1.0f);
	camera = glm::lookAt(eye, center, up);
	this->theta = theta;
	this->phi = phi;
	this->R = R;
}

void Renderer::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void renderObject
		(const glm::vec3 &r, float scale, const Mesh &object,
		GLuint program, glm::mat4 camera, glm::mat4 projection, glm::vec4 color);

void renderOrbit
		(const KeplerianElements &ke, const Circle &circle,
		GLuint program, glm::mat4 camera, glm::mat4 projection, glm::vec4 color);

void Renderer::render(Object object)
{
	if(object.isActive())
	{
		renderObject
			(object.get_r(),
			 0.02f, this->object,
			 program,
			 camera, projection, colorObject);
	}
	if(!object.isCrashed())
	{
		object.deactivate();
		renderOrbit
			(object.get_ke(),
			circle, program,
			camera, projection, colorOrbit);
	}
}

void Renderer::renderEarth()
{
	/*
	glm::vec4 colorEquator(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 colorMeridian(1.0f, 1.0f, 1.0f, 1.0f);
	glm::mat4 modelViewEquator = camera;
	circle.render(program, modelViewEquator, projection, colorEquator, GL_LINE_LOOP);

	glm::vec3 x_axis(1.0f, 0.0f, 0.0f);
	glm::vec3 y_axis(0.0f, 1.0f, 0.0f);
	glm::vec3 z_axis(0.0f, 0.0f, 1.0f);
	float angle = M_PI/2;

	glm::mat4 modelViewMeridian =
				camera *
				glm::rotate(glm::mat4(1.0f), angle, x_axis);
	circle.render(program, modelViewMeridian, projection, colorMeridian, GL_LINE_LOOP);
	*/
	glm::vec4 colorEarth(0.4f, 0.4f, 0.4f, 1.0f);
	glm::mat4 modelViewEarth =
				camera;
	earth.render(program, modelViewEarth, projection, colorEarth, GL_TRIANGLES);
}

void renderObject
		(const glm::vec3 &r, float scale, const Mesh &object,
		GLuint program, glm::mat4 camera, glm::mat4 projection, glm::vec4 color)
{
	glm::vec3 scale_vec(scale, scale, scale);
	glm::mat4 modelView =
			camera *
			glm::translate(glm::mat4(1.0f), r/R_earth) *
			glm::scale(glm::mat4(1.0f), scale_vec);
	object.render(program, modelView, projection, color, GL_TRIANGLES);
}

glm::mat4 circleToOrbit
		(float e, float a,
		float inc, float LAN, float AP);

void renderOrbit
		(const KeplerianElements &ke, const Circle &circle,
		GLuint program, glm::mat4 camera, glm::mat4 projection, glm::vec4 color)
{
	glm::mat4 orbitTransform = circleToOrbit(ke.e, ke.a/R_earth, ke.inc, ke.LAN, ke.AP);
	glm::mat4 modelViewOrbit = camera * orbitTransform;
	circle.render(program, modelViewOrbit, projection, color, GL_LINE_LOOP);
}

glm::mat4 circleToOrbit
		(float e, float a,
		float inc, float LAN, float AP)
{
	float b = a * sqrt(1 - e*e); /* Semi minor axis */
	float f = a * e; /* Distance from the center to the focus. */
	glm::vec3 scale(a, b, 1.0f); /* Scale the circle */
	glm::vec3 translation(-f, 0.0f, 0.0f); /* Move up by one focus distance. */

	glm::vec3 x_axis(1.0f, 0.0f, 0.0f);
	glm::vec3 y_axis(0.0f, 1.0f, 0.0f);
	glm::vec3 z_axis(0.0f, 0.0f, 1.0f);
	glm::mat4 model =
		glm::rotate(glm::mat4(1.0f), LAN, z_axis) * 
		glm::rotate(glm::mat4(1.0f), inc, x_axis) *
		glm::rotate(glm::mat4(1.0f), AP, z_axis) *
		glm::translate(glm::mat4(1.0f), translation) *
		glm::scale(glm::mat4(1.0f), scale);
	return model;
}
