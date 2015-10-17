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

float data[] = {
 0.0f,  0.5f, 0.0f,
-0.5f, -0.5f, 0.0f,
 0.5f, -0.5f, 0.0f };

GLuint buffer_id;

GLuint vshader_id;
GLuint fshader_id;
GLuint shader_program_id;

const char * vertex_shader_filename = "vertex_shader.vert";
const char * fragment_shader_filename = "fragment_shader.frag";

void render_init()
{
	glGenBuffers(1, &buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glClearColor(0.03, 0.03, 0.03, 1.0);
	vshader_id = loadShader(GL_VERTEX_SHADER, vertex_shader_filename);
	fshader_id = loadShader(GL_FRAGMENT_SHADER, fragment_shader_filename);
	shader_program_id = loadProgram(vshader_id, fshader_id);
}

void render()
{
	glm::vec3 axis = glm::vec3(0.0, 1.0, 0.0);
	glm::vec3 translation = glm::vec3(0.0, 0.0, -3.0);
	glm::vec3 scale = glm::vec3(2.0, 2.0, 2.0);
	glm::vec3 eye = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 center = glm::vec3(0.0, 0.0, -1.0);
	glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
	glm::mat4 modelView =
			glm::translate(glm::mat4(1.0), translation) *
			glm::scale(glm::mat4(1.0), scale) *
			glm::rotate(glm::mat4(1.0), angle, axis);
	glm::mat4 perspective = glm::perspective(M_PI/2, 1.0, 0.0, 10.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shader_program_id);
	glUniformMatrix4fv(0, 1, GL_FALSE, &modelView[0][0]);
	glUniformMatrix4fv(1, 1, GL_FALSE, &perspective[0][0]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_LINE_LOOP, 0, 3);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);
}

void render_shutdown()
{
	glDeleteShader(vshader_id);
	glDeleteShader(fshader_id);
	glDeleteProgram(shader_program_id);
	glDeleteBuffers(1, &buffer_id);
}
