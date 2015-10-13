#include "SDL.h"
#include <gl.h>

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

void render_init()
{
	glClearColor(0.1, 0.1, 0.1, 1.0);
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void render_shutdown()
{}
