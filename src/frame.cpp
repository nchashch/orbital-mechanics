#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <glm.hpp>
#include "SDL.h"
#define GL_GLEXT_PROTOTYPES
#include <glcorearb.h>
#include "Object.h"
#include "Mesh.h"
#include "Renderer.h"
#include "constants.h"

float zoom_rate = 20.0f;
float drag_rate = M_PI/2;
float R_min = 2.0f;
float R_max = 100.0f;
float R = R_min;
int time_warp = 0;
const double far = 300.0f;
const double near = 0.1f;
float theta = 0.0f;
float phi = M_PI/2;

std::vector<Object> objects;

/*
name   e   a     inc  LAN AP  trueAnomaly m   Cd  A
Object 0.4 2.0e6 1.57 1.4 4.3 2.1         1.0 1.0 1.0
*/

void frame_init()
{
	const char* objects_filename = "objects";
	std::ifstream obj_ifs(objects_filename);
	if(obj_ifs.is_open())
	{
		std::string line;
 		while(std::getline(obj_ifs, line))
		{
			std::istringstream iss(line);
			std::string name;
			KeplerianElements ke;
			float m, Cd, A;
			iss >> name >> ke.e >> ke.a >> ke.inc >> ke.LAN >> ke.AP;
			ke.LOP = ke.LAN + ke.AP;
			float trueAnomaly;
			iss >> trueAnomaly;
			float E = 2*atan(tan(trueAnomaly/2)/sqrt((1+ke.e)/(1-ke.e)));
			ke.M0 = E - ke.e * sin(E);
			ke.epoch = 0.0f;
			ke.t = 0.0f;
			iss >> m >> Cd >> A;
			objects.push_back(Object(name, ke, m, Cd, A));
		}
	}
}

const float delay = 0.1f;
float timer = 0.0f;

int frame
	(float dt,
	SDL_Event evt,
	const Uint8* kbd,
	float dx, float dy, Uint32 mouseb)
{
	float real_tw = pow(10.0, time_warp);
	for(std::vector<Object>::iterator i = objects.begin(); i != objects.end(); ++i)
	{
		i->tick(dt*real_tw);
	}

	if(mouseb & SDL_BUTTON(SDL_BUTTON_LEFT))
	{
		theta -= dx * drag_rate;
		phi += dy * drag_rate;
		if(phi < 0.0f)
		{
			phi = 0.001f;
		}
		if(phi > M_PI)
		{
			phi = M_PI - 0.001f;
		}
	}

	if(kbd[SDL_SCANCODE_PERIOD] && timer > delay && time_warp < 3)
	{
		time_warp += 1;
		timer = 0.0f;
	}
	if(kbd[SDL_SCANCODE_COMMA] && timer > delay && time_warp > 0)
	{
		time_warp -= 1;
		timer = 0.0f;
	}

	timer += dt;

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
		for(std::vector<Object>::iterator i = objects.begin(); i != objects.end(); ++i)
		{
			i->activate();
		}
	}
	if(kbd[SDL_SCANCODE_D])
	{
		for(std::vector<Object>::iterator i = objects.begin(); i != objects.end(); ++i)
		{
			i->deactivate();
		}
	}

	if(kbd[SDL_SCANCODE_ESCAPE])
	{
		return 1;
	}

	return 0;
}

void frame_shutdown()
{
}

Renderer *renderer;
const char * vertex_shader_filename = "vertex_shader.vert";
const char * fragment_shader_filename = "fragment_shader.frag";

void render_init(int wnd_w, int wnd_h)
{
	float aspect = (float)wnd_w / (float)wnd_h;
	renderer = new Renderer
			(std::string(vertex_shader_filename),
			std::string(fragment_shader_filename),
			std::string("object.obj"),
			std::string("sphere.obj"),
			M_PI/4, aspect,
			near, far);
}

void render()
{
	renderer->frameTick(theta, phi, R);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for(std::vector<Object>::iterator i = objects.begin(); i != objects.end(); ++i)
	{
		renderer->render(*i);
	}
	renderer->renderEarth();
}

void render_shutdown()
{
	delete renderer;
}
