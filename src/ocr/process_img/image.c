#include "image.h"
#include "detection.h"

#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

Image *load_png(const char *filename)
{
    SDL_Surface *src = IMG_Load(filename);

    if (src == NULL)
    {
        SDL_Log("%s", IMG_GetError());
        return NULL;
    }

    SDL_Surface *surface =
        SDL_ConvertSurfaceFormat(
            src,
            SDL_PIXELFORMAT_RGBA32,
            0);

    SDL_FreeSurface(src);

    Image *img = malloc(sizeof(Image));

    img->width = surface->w;
    img->height = surface->h;

    img->pixels = malloc(img->width * img->height);

    SDL_LockSurface(surface);

    Uint8 *pixels = (Uint8 *)surface->pixels;

    for (int y = 0; y < img->height; y++)
    {
        for (int x = 0; x < img->width; x++)
        {
            Uint8 *p = pixels + y * surface->pitch + x * 4;

            int gray = (p[0] + p[1] + p[2]) / 3;

            img->pixels[y * img->width + x] =
                (gray < 210) ? 1 : 0;
        }
    }

    SDL_UnlockSurface(surface);

    SDL_FreeSurface(surface);

	img->squares = NULL;
	img->valid_squares = NULL;

    return img;
}

void process_image(Image *img)
{
	img->squares = get_squares_sudoku(img);
	img->valid_squares = get_valid_squares(img->squares);
}

void free_image(Image *img)
{
    if (img == NULL)
        return;

    free(img->pixels);
	if (img->squares)
	{
		for (int i = 0; i < 81; i++)
			free(img->squares[i]);
	}
	free(img->squares);
	free(img->valid_squares);
    free(img);
}

uint8_t get_pixel(Image *img, int x, int y)
{
    return img->pixels[y * img->width + x];
}

void set_pixel(Image *img,
               int x,
               int y,
               uint8_t value)
{
    img->pixels[y * img->width + x] = value;
}
