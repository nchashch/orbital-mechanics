#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <glcorearb.h>
#include "Circle.h"

void renderEarth
		(GLuint program,
		glm::mat4 camera, glm::mat4 projection,
		const Circle &circle,
	       	float phi, float theta)
{
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

	glm::vec4 colorEarth(0.04f, 0.1f, 0.6f, 1.0f);
	glm::vec4 colorAtmosphere(0.07, 0.2f, 0.7f, 0.6f);
	glm::mat4 modelViewEarth =
				camera *
				glm::rotate(glm::mat4(1.0f), theta, z_axis) *
				glm::rotate(glm::mat4(1.0f), phi, y_axis);
	circle.render(program, modelViewEarth, projection, colorEarth, GL_TRIANGLE_FAN);
}
