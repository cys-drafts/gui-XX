#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <assert.h>

int main(void)
{
	SDL_Window *win;
	SDL_Surface *sfc;
	SDL_Surface *png;
	SDL_Renderer *render;
	SDL_Texture *text;	

	SDL_Init(SDL_INIT_VIDEO);
	win = SDL_CreateWindow("png", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
	assert(win);
	render = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	assert(render);
	SDL_SetRenderDrawColor(render, 0xff, 0xff, 0xff, 0xff);
	IMG_Init(IMG_INIT_PNG);
	sfc = SDL_GetWindowSurface(win);

	png = IMG_Load("./preview.png");
	assert(png);
	text = SDL_CreateTextureFromSurface(render, png);

	SDL_RenderClear(render);
	SDL_RenderCopy(render, text, NULL, NULL);
	SDL_RenderPresent(render);

	SDL_Delay(2000);

	SDL_FreeSurface(png);
	SDL_DestroyTexture(text);
	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(win);
	IMG_Quit();
	SDL_Quit();

	return 0;
}
