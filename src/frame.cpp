#include "SDL.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#define GL_GLEXT_PROTOTYPES
#include <glcorearb.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "loadShader.h"
#include "Circle.h"

float angle;
const float angle_change_rate = 10.25f;

void frame_init()
{
	angle = 0.0f;
}

int frame
	(float dt,
	const Uint8* kbd,
	float mousex, float mousey, Uint32 mouseb)
{
	angle += dt * angle_change_rate;
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
}

void render()
{
	glm::vec3 axis = glm::vec3(1.0, 0.0, 0.0);
	glm::vec3 translation = glm::vec3(0.0, 0.0, -10.0);
	glm::vec3 scale = glm::vec3(1.0, 2.0, 1.0);
	glm::vec3 eye = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 center = glm::vec3(0.0, 0.0, -1.0);
	glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
	glm::mat4 modelView =
			glm::translate(glm::mat4(1.0), translation) *
			glm::rotate(glm::mat4(1.0), angle, axis) *
			glm::scale(glm::mat4(1.0), scale);
	glm::mat4 perspective = glm::perspective(M_PI/4, 1.0, 0.0, 100.0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shader_program_id);
	glUniformMatrix4fv(0, 1, GL_FALSE, &modelView[0][0]);
	glUniformMatrix4fv(1, 1, GL_FALSE, &perspective[0][0]);
	circle->render();
	glUseProgram(0);
}

void render_shutdown()
{
	glDeleteShader(vshader_id);
	glDeleteShader(fshader_id);
	glDeleteProgram(shader_program_id);
	delete circle;
}
