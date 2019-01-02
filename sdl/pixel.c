#include <SDL2/SDL.h>
#include <stdio.h>

int main(void)
{
	SDL_Window *win;
	SDL_Surface *sfc;
	SDL_Surface *img;
	SDL_Event e;
	SDL_Rect rect;
	int i;
	uint32_t *pixels;

	SDL_Init(SDL_INIT_VIDEO);
	win = SDL_CreateWindow("pixel", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
	sfc = SDL_GetWindowSurface(win);
	printf("pitch %d Bpp %d\n", sfc->pitch, sfc->format->BytesPerPixel);

	img = SDL_LoadBMP("./left.bmp");
	printf("pitch %d Bpp %d\n", img->pitch, img->format->BytesPerPixel);

	SDL_BlitSurface(img, NULL, sfc, NULL);
	SDL_UpdateWindowSurface(win);
	SDL_FreeSurface(img);

	SDL_FillRect(sfc, NULL, SDL_MapRGB(sfc->format, 0xff, 0xff, 0xff));
#if 1
	rect.x = 0;
	rect.y = 0;
	rect.w = 640;
	rect.h = 1;
	SDL_FillRect(sfc, &rect, SDL_MapRGB(sfc->format, 0xff, 0x00, 0x00));
#else
	pixels = sfc->pixels;
	for (i = 0; i < sfc->pitch/sfc->format->BytesPerPixel; i++) {
		*pixels = SDL_MapRGB(sfc->format, 0xff, 0x00, 0x00);
		pixels++;
	}
#endif
	SDL_UpdateWindowSurface(win);

	while (1) {
		if (SDL_WaitEvent(&e) == 1) {
			if (e.type == SDL_QUIT)
				break;
		}
	}

	SDL_DestroyWindow(win);
	return 0;
}
