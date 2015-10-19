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

Object *object1;
Object *object2;

void frame_init()
{
	object1 = new Object
			("Test object1",
			glm::vec3(0.0f, 1.1f, 0.0f) * R_earth,
			glm::vec3(10.0e3f, 0.0f, 1.0e-12f), /* v.z shouldn't be zero */
			glm::vec3(0.0f, 0.0f, 0.0f),
			9.0f, 1.0f, 1.0f, 1.0f);

	object2 = new Object
			("Test object2",
			glm::vec3(0.5f, -2.1f, 0.4f) * R_earth,
			glm::vec3(-2.0e3f, 0.0f, 8.0e3f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			0.0f, 1.0f, 1.0f, 1.0f);
}

int frame
	(float dt,
	SDL_Event evt,
	const Uint8* kbd,
	float dx, float dy, Uint32 mouseb)
{
	object1->tick(dt);
	object2->tick(dt);

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

	if(kbd[SDL_SCANCODE_A])
	{
		object1->activate();
		object2->activate();

	}
	if(kbd[SDL_SCANCODE_D])
	{
		object1->deactivate();
		object2->deactivate();
	}

	if(kbd[SDL_SCANCODE_ESCAPE])
	{
		return 1;
	}

	return 0;
}

void frame_shutdown()
{
	delete object1;
	delete object2;
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
	renderer->render(*object1);
	renderer->render(*object2);
	renderer->renderEarth();
}

void render_shutdown()
{
	delete renderer;
}
