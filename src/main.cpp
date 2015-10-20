#include "SDL.h"
#define DELAY_TIME 20

void frame_init();
int frame
	(float dt,
	SDL_Event evt,
	const Uint8* kbd,
	float dx, float dy, Uint32 mouseb);
void frame_shutdown();
void render_init(int wnd_w, int wnd_h);
void render();
void render_shutdown();

int main(int argc, char**argv)
{
	if(SDL_Init(SDL_INIT_VIDEO)==-1)
	{
		SDL_Log("%s\n", SDL_GetError());
	}
	SDL_Window* wnd;
	wnd = SDL_CreateWindow
			("",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			1680, 1050,
			SDL_WINDOW_OPENGL);
	if(wnd == NULL)
	{
		SDL_Log("%s\n", SDL_GetError());
	}
	SDL_GLContext glctxt = SDL_GL_CreateContext(wnd);
	if(glctxt == NULL)
	{
		SDL_Log("%s\n", SDL_GetError());
	}
	const Uint8* kbd = SDL_GetKeyboardState(NULL);
	if(kbd == NULL)
	{
		SDL_Log("%s\n", SDL_GetError());
	}

	int done=0;
	float t=0.0;
	float dt=0.0;
	frame_init();
	int wnd_w, wnd_h;
	SDL_GetWindowSize(wnd, &wnd_w, &wnd_h);
	render_init(wnd_w, wnd_h);

	float mousex_old;
	float mousey_old;

	while(!done)
	{
		int start_time = SDL_GetTicks();
		/* react to system quit event */
		SDL_Event evt;
		while(SDL_PollEvent(&evt))
		{
			if(evt.type==SDL_QUIT)
			{
				done = 1;
			}
		}

		/* get mouse state */
		int imousex, imousey; /* i for int */
		Uint32 mouseb = SDL_GetMouseState(&imousex, &imousey);
		/* convert "integer" coordinates to "floating point" coordinates */
		/* (0.5, 0.5) is monitor center */
		/* (1.0, 1.0) is top right corner */
		/* (0.0, 0.0) is bottom left corner */
		float mousex, mousey;
		int wnd_w, wnd_h;
		SDL_GetWindowSize(wnd, &wnd_w, &wnd_h);
		mousex = (float)(imousex)/(wnd_w);
		mousey = 1.0f - (float)(imousey)/(wnd_h);

		float dx = mousex - mousex_old;
		float dy = mousey - mousey_old;

		mousex_old = mousex;
		mousey_old = mousey;

		done = frame(dt, evt, kbd, dx, dy, mouseb);
		render();
		SDL_GL_SwapWindow(wnd);
		/* sleep */
		int dt_int = SDL_GetTicks() - start_time;
		if(dt_int < DELAY_TIME)
			SDL_Delay(DELAY_TIME - dt_int);
		dt = (float)dt_int / 1.0e3;
	}
	frame_shutdown();
	render_shutdown();

	/* free resources */
	SDL_GL_DeleteContext(glctxt);
	SDL_DestroyWindow(wnd);
	SDL_Quit();

	return 0;
}
