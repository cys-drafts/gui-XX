#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <assert.h>

int main(void)
{
	SDL_Window *win;
	SDL_Surface *sfc;
	SDL_Surface *png;

	SDL_Init(SDL_INIT_VIDEO);
	win = SDL_CreateWindow("png", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
	assert(win);
	IMG_Init(IMG_INIT_PNG);
	sfc = SDL_GetWindowSurface(win);

	png = IMG_Load("./texture.png");
	assert(png);

	SDL_BlitSurface(png, NULL, sfc, NULL);
	SDL_UpdateWindowSurface(win);

	SDL_Delay(2000);

	SDL_FreeSurface(png);
	SDL_DestroyWindow(win);
	IMG_Quit();
	SDL_Quit();

	return 0;
}
