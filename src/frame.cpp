#include "SDL.h"
#include <iostream>
#include <string>
#include <glm.hpp>
#define GL_GLEXT_PROTOTYPES
#include <glcorearb.h>
#include "Object.h"
#include "Renderer.h"
#include "constants.h"

float zoom_rate = 40.0f;
float drag_rate = M_PI;
float R_min = 10.0f;
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
			glm::vec3(0.0f, 1.1f, 0.0f) * R_earth,
			glm::vec3(10.0e3f, 1.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			t, 1.0f, 1.0f, 1.0f);

	KeplerianElements ke = object->get_KeplerianElements();
	std::cout << "e = " << ke.e << std::endl;
	std::cout << "a = " << ke.a << std::endl;
	std::cout << "inc = " << ke.inc << std::endl;
	std::cout << "LAN = " << ke.LAN << std::endl;
	std::cout << "AP = " << ke.AP << std::endl;
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

Renderer *renderer;
const char * vertex_shader_filename = "vertex_shader.vert";
const char * fragment_shader_filename = "fragment_shader.frag";

void render_init()
{
	renderer = new Renderer
			(std::string(vertex_shader_filename),
			std::string(fragment_shader_filename),
			M_PI/4, 1366.0/768.0,
			near, far);
}

void render()
{
	renderer->frameTick(theta, phi, R);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderer->render(*object);
	renderer->renderEarth();
}

void render_shutdown()
{
	delete renderer;
}
