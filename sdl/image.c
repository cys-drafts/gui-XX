#include <SDL2/SDL.h>
#include <stdio.h>
#include <assert.h>

enum {
	KEY_SURFACE_LEFT,
	KEY_SURFACE_RIGHT,
	KEY_SURFACE_TOTAL,
};

SDL_Window *window;
SDL_Surface *surface;
SDL_Surface *current;
SDL_Surface *img_surface[KEY_SURFACE_TOTAL];

#define die(s)	do {fprintf(stderr, "%s\n", s); exit(-1);} while (0)

void init(void)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		die("init");
	window = SDL_CreateWindow("image", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
	if (!window)
		die("window");
	surface = SDL_GetWindowSurface(window);
}

void load_media(void)
{
	SDL_Surface *raw_surface;
	raw_surface = SDL_LoadBMP("./left.bmp");
	assert(raw_surface);
	img_surface[KEY_SURFACE_LEFT] = SDL_ConvertSurface(raw_surface, surface->format, 0);
	if (!img_surface[KEY_SURFACE_LEFT]) {
		die(SDL_GetError());
	}
	SDL_FreeSurface(raw_surface);
	raw_surface = SDL_LoadBMP("./right.bmp");
	assert(raw_surface);
	img_surface[KEY_SURFACE_RIGHT] = SDL_ConvertSurface(raw_surface, surface->format, 0);
	if (!img_surface[KEY_SURFACE_RIGHT]) {
		die(SDL_GetError());
	}
	SDL_FreeSurface(raw_surface);
}

void fini(void)
{
	SDL_FreeSurface(img_surface[KEY_SURFACE_LEFT]);
	SDL_FreeSurface(img_surface[KEY_SURFACE_RIGHT]);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int main(int argc, char **argv)
{
	int quit = 0;
	SDL_Event e;
	SDL_Rect stretch_rect;

	init();
	load_media();

	SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0xff, 0xff, 0xff));
	SDL_UpdateWindowSurface(window);

#if 0
	while (1) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT)
				quit = 1;
		}
	}
#else
	stretch_rect.x = 0;
	stretch_rect.y = 0;
	stretch_rect.w = 640;
	stretch_rect.h = 480;

	while (1) {
		if (SDL_WaitEvent(&e) == 1) {
			if (e.type == SDL_QUIT)
				break;
			else if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
					case SDLK_LEFT:
						current = img_surface[KEY_SURFACE_LEFT];
						break;
					case SDLK_RIGHT:
						current = img_surface[KEY_SURFACE_RIGHT];
						break;
					default:
						break;
				}
				SDL_BlitScaled(current, NULL, surface, &stretch_rect);
				SDL_UpdateWindowSurface(window);
			}
		}
	}
#endif

	fini();
	return 0;
}
