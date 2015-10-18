#include "SDL.h"
#include <cmath>
#include <iostream>
#define GL_GLEXT_PROTOTYPES
#include <glcorearb.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "loadShader.h"
#include "Circle.h"
#include "Object.h"
#include "renderEarth.h"
#include "constants.h"

glm::vec3 eye;
glm::vec3 center;
glm::vec3 up;

float zoom_rate = 40.0f;
float drag_rate = M_PI;
float R_min = 5.0f;
float R_max = 100.0f;
float R = R_min;
const double far = 300.0f;
const double near = 0.1f;
float theta = 0.0f;
float phi = M_PI/2;

Object *object;
float t = 0.0f;
void frame_init()
{
	object = new Object
			("Test object",
			glm::vec3(0.0f, 2.0f, 1.0f) * R_earth,
			glm::vec3(7.1e3f, 0.0f, 2.0e3f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			t, 1.0f, 1.0f, 1.0f);
	KeplerianElements ke = object->get_KeplerianElements();

	std::cout << "e = " << ke.e << std::endl;
	std::cout << "a = " << ke.a << std::endl;
	std::cout << "inc = " << ke.inc << std::endl;
	std::cout << "LAN = " << ke.LAN << std::endl;
	std::cout << "AP = " << ke.AP << std::endl;
	
	eye = glm::vec3(1.0f, 0.0f, 0.0f);
	center = glm::vec3(0.0f, 0.0f, 0.0f);
	up = glm::vec3(0.0f, 0.0f, 1.0f);
}

int frame
	(float dt,
	SDL_Event evt,
	const Uint8* kbd,
	float dx, float dy, Uint32 mouseb)
{
	t += dt;
	object->tick(dt);
	object->recompute_ke(t);
	if(mouseb & SDL_BUTTON(SDL_BUTTON_LEFT))
	{
		theta -= dx * drag_rate;
		phi += dy * drag_rate;
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
{
	delete object;
}

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

void render()
{
	glm::mat4 camera = glm::lookAt(eye, center, up);

	float aspect = 1366.0f/768.0f;
	glm::mat4 projection = glm::perspective(M_PI/4, 1366.0/768.0, near, far);

	glm::vec4 colorOrbit(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 colorObject(1.0f, 0.3f, 0.3f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderOrbit
		(object->get_KeplerianElements(),
		*circle, shader_program_id,
		camera, projection, colorOrbit);
	renderObject
		(object->get_r(),
		 0.1f, *circle,
		 shader_program_id,
		 camera, projection, colorObject);
	renderEarth(shader_program_id, camera, projection, *circle, phi, theta);
}

void render_shutdown()
{
	glDeleteShader(vshader_id);
	glDeleteShader(fshader_id);
	glDeleteProgram(shader_program_id);
	delete circle;
}
