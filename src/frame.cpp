#include "SDL.h"
#include <cmath>
#define GL_GLEXT_PROTOTYPES
#include <glcorearb.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "loadShader.h"
#include "Circle.h"

glm::vec3 eye;
glm::vec3 center;
glm::vec3 up;

float zoom_rate = 40.06f;
float R_min = 10.0f;
float R_max = 40.0f;
float R = R_min;
const double far = 100.0f;
const double near = 0.1f;
float theta = 0.0f;
float phi = M_PI/2;

void frame_init()
{
	eye = glm::vec3(1.0f, 0.0f, 0.0f);
	center = glm::vec3(0.0f, 0.0f, 0.0f);
	up = glm::vec3(0.0f, 0.0f, 1.0f);
}

int frame
	(float dt,
	const Uint8* kbd,
	float dx, float dy, Uint32 mouseb)
{
	if(mouseb & SDL_BUTTON(SDL_BUTTON_LEFT))
	{
		theta -= dx * 2 * M_PI;
		phi += dy * 2 * M_PI;
		if(phi < 0.0f)
		{
			phi = 0.001f;
		}
		if(phi > M_PI / 2)
		{
			phi = M_PI / 2 - 0.001f;
		}
	}

	float x, y, z;
	x = R * sin(phi) * cos(theta);
	y = R * sin(phi) * sin(theta);
	z = R * cos(phi);
	eye = glm::vec3(x, y, z);

	if(kbd[SDL_SCANCODE_UP] && R > R_min)
	{
		R -= zoom_rate * dt;
	}

	if(kbd[SDL_SCANCODE_DOWN] && R < R_max)
	{
		R += zoom_rate * dt;
	}

	if(kbd[SDL_SCANCODE_ESCAPE])
	{
		return 1;
	}
	return 0;
}

void frame_shutdown()
{}

GLuint vshader_id;
GLuint fshader_id;
GLuint shader_program_id;

const char * vertex_shader_filename = "vertex_shader.vert";
const char * fragment_shader_filename = "fragment_shader.frag";

Circle *circle;

void render_init()
{
	circle = new Circle(64);
	glClearColor(0.03, 0.03, 0.03, 1.0);
	vshader_id = loadShader(GL_VERTEX_SHADER, vertex_shader_filename);
	fshader_id = loadShader(GL_FRAGMENT_SHADER, fragment_shader_filename);
	shader_program_id = loadProgram(vshader_id, fshader_id);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glLineWidth(2.0f);
}

glm::mat4 circleToOrbit
		(float e, float a,
		float inc, float LAN, float AP);
void render()
{
	glm::mat4 orbitTransform = circleToOrbit
					(0.3f, 4.7f,
					 0.3f, 0.8f, 0.6f);
	glm::mat4 camera = glm::lookAt(eye, center, up);
	glm::mat4 modelViewOrbit =
				camera *
				orbitTransform;

	float aspect = 1366.0f/768.0f;
	glm::mat4 projection = glm::perspective(M_PI/4, 1366.0/768.0, near, far);
	glm::vec4 colorOrbit(1.0f, 1.0f, 1.0f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::vec4 colorEquator(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 colorMeridian(1.0f, 1.0f, 1.0f, 1.0f);
	circle->render(shader_program_id, modelViewOrbit, projection, colorOrbit, GL_LINE_LOOP);
	glm::mat4 modelViewEquator = camera;
	circle->render(shader_program_id, modelViewEquator, projection, colorEquator, GL_LINE_LOOP);

	glm::vec3 x_axis(1.0f, 0.0f, 0.0f);
	glm::vec3 y_axis(0.0f, 1.0f, 0.0f);
	glm::vec3 z_axis(0.0f, 0.0f, 1.0f);
	float angle = M_PI/2;

	glm::mat4 modelViewMeridian =
				camera *
				glm::rotate(glm::mat4(1.0f), angle, y_axis);
	circle->render(shader_program_id, modelViewMeridian, projection, colorMeridian, GL_LINE_LOOP);

	glm::vec4 colorEarth(0.04f, 0.1f, 0.6f, 1.0f);
	glm::vec4 colorAtmosphere(0.07, 0.2f, 0.7f, 0.6f);
	glm::mat4 modelViewEarth =
				camera *
				glm::rotate(glm::mat4(1.0f), theta, z_axis) *
				glm::rotate(glm::mat4(1.0f), phi, y_axis);
	circle->render(shader_program_id, modelViewEarth, projection, colorEarth, GL_TRIANGLE_FAN);
}

glm::mat4 circleToOrbit
		(float e, float a,
		float inc, float LAN, float AP)
{
	float b = a * sqrt(1 - e*e); /* Semi minor axis */
	float f = a * e; /* Distance from the center to the focus. */
	glm::vec3 scale(b, a, 1.0f); /* Scale the circle */
	glm::vec3 translation(0.0f, f, 0.0f); /* Move up by one focus distance. */

	glm::vec3 x_axis(1.0f, 0.0f, 0.0f);
	glm::vec3 z_axis(0.0f, 0.0f, 1.0f);

	glm::mat4 model =
		glm::rotate(glm::mat4(1.0f), LAN, z_axis) *
		glm::rotate(glm::mat4(1.0f), inc, x_axis) *
		glm::rotate(glm::mat4(1.0f), AP, z_axis) *
		glm::translate(glm::mat4(1.0f), translation) *
		glm::scale(glm::mat4(1.0f), scale);
	return model;
}

void render_shutdown()
{
	glDeleteShader(vshader_id);
	glDeleteShader(fshader_id);
	glDeleteProgram(shader_program_id);
	delete circle;
}
