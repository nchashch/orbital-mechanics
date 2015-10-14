#include "SDL.h"
#include <stdlib.h>
#define GL_GLEXT_PROTOTYPES
#include <glcorearb.h>

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

const char* vshader_src =
"\
";

const char* fshader_src =
"\
";

GLuint vshader_id;
GLuint fshader_id;
GLuint shader_program_id;

void render_init()
{
	glClearColor(0.1, 0.1, 0.1, 1.0);

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
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void render_shutdown()
{}
