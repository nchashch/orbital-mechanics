#include "SDL.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#define GL_GLEXT_PROTOTYPES
#include <glcorearb.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

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

const char * vertex_shader_filename = "vertex_shader";
const char * fragment_shader_filename = "fragment_shader";

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

	std::ifstream vertex_ifs(vertex_shader_filename);
	std::string vshader_src = "";
	if(vertex_ifs.is_open())
	{
		std::string buf = "";
		while(std::getline(vertex_ifs, buf))
		{
			vshader_src += buf;
			vshader_src += '\n';
		}
		vertex_ifs.close();
	}

	std::ifstream fragment_ifs(fragment_shader_filename);
	std::string fshader_src = "";
	if(fragment_ifs.is_open())
	{
		std::string buf = "";
		while(std::getline(fragment_ifs, buf))
		{
			fshader_src += buf;
			fshader_src += '\n';
		}
		fragment_ifs.close();
	}

	GLint shader_src_length;
	GLint info_log_length;
	char* info_log = NULL;

	vshader_id = glCreateShader(GL_VERTEX_SHADER);
	shader_src_length = vshader_src.length();
	char const * vshader_src_ptr = vshader_src.c_str();
	glShaderSource(vshader_id, 1, &vshader_src_ptr, &shader_src_length);
	glCompileShader(vshader_id);

	glGetShaderiv(vshader_id, GL_INFO_LOG_LENGTH, &info_log_length);
	if(info_log_length > 0)
	{
		info_log = (char*)malloc(info_log_length + 1);
		info_log[info_log_length] = '\0';
		glGetShaderInfoLog(vshader_id, info_log_length, NULL, info_log);
		std::cout << info_log;
		free(info_log);
		info_log = NULL;
	}

	fshader_id = glCreateShader(GL_FRAGMENT_SHADER);
	shader_src_length = fshader_src.length();
	char const * fshader_src_ptr = fshader_src.c_str();
	glShaderSource(fshader_id, 1, &fshader_src_ptr, &shader_src_length);
	glCompileShader(fshader_id);

	glGetShaderiv(fshader_id, GL_INFO_LOG_LENGTH, &info_log_length);
	if(info_log_length > 0)
	{
		info_log = (char*)malloc(info_log_length + 1);
		info_log[info_log_length] = '\0';
		glGetShaderInfoLog(fshader_id, info_log_length, NULL, info_log);
		std::cout << info_log;
		free(info_log);
		info_log = NULL;
	}

	shader_program_id = glCreateProgram();
	glAttachShader(shader_program_id, vshader_id);
	glAttachShader(shader_program_id, fshader_id);
	glLinkProgram(shader_program_id);

	glGetProgramiv(shader_program_id, GL_INFO_LOG_LENGTH, &info_log_length);
	if(info_log_length > 0)
	{
		info_log = (char*)malloc(info_log_length + 1);
		info_log[info_log_length] = '\0';
		glGetProgramInfoLog(shader_program_id, info_log_length, NULL, info_log);
		std::cout << info_log;
		free(info_log);
		info_log = NULL;
	}
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
