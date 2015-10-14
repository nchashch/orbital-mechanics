#include "SDL.h"
#include <stdlib.h>
#define GL_GLEXT_PROTOTYPES
#include <glcorearb.h>
#include "text.h"
#include "matrix.h"
#include "transformation.h"

void frame_init()
{}

int frame
	(double dt,
	const Uint8* kbd,
	double mousex, double mousey, Uint32 mouseb)
{
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

	const char* vshader_src = load_file("vertex_shader");
	const char* fshader_src = load_file("fragment_shader");

	GLint shader_src_length;
	GLint info_log_length;
	char* info_log = NULL;

	vshader_id = glCreateShader(GL_VERTEX_SHADER);
	shader_src_length = strlen(vshader_src);
	glShaderSource(vshader_id, 1, &vshader_src, &shader_src_length);
	glCompileShader(vshader_id);

	glGetShaderiv(vshader_id, GL_INFO_LOG_LENGTH, &info_log_length);
	if(info_log_length > 0)
	{
		info_log = (char*)malloc(info_log_length + 1);
		info_log[info_log_length] = '\0';
		glGetShaderInfoLog(vshader_id, info_log_length, NULL, info_log);
		printf("%s\n", info_log);
		free(info_log);
		info_log = NULL;
	}

	fshader_id = glCreateShader(GL_FRAGMENT_SHADER);
	shader_src_length = strlen(fshader_src);
	glShaderSource(fshader_id, 1, &fshader_src, &shader_src_length);
	glCompileShader(fshader_id);

	glGetShaderiv(fshader_id, GL_INFO_LOG_LENGTH, &info_log_length);
	if(info_log_length > 0)
	{
		info_log = (char*)malloc(info_log_length + 1);
		info_log[info_log_length] = '\0';
		glGetShaderInfoLog(fshader_id, info_log_length, NULL, info_log);
		printf("%s\n", info_log);
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
		printf("%s\n", info_log);
		free(info_log);
		info_log = NULL;
	}

	matrix_t modelview = identity();
	matrix_t projection = identity();
	glUseProgram(shader_program_id);
	glUniformMatrix4fv(0, 1, GL_FALSE, modelview.d);
	glUniformMatrix4fv(1, 1, GL_FALSE, projection.d);
	glUseProgram(0);
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shader_program_id);
	glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
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
