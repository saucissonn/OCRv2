#include "image.h"

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

    if (surface == NULL)
    {
        SDL_Log("%s", SDL_GetError());
        return NULL;
    }

    Image *img = malloc(sizeof(Image));

    if (img == NULL)
    {
        SDL_FreeSurface(surface);
        return NULL;
    }

    img->width = surface->w;
    img->height = surface->h;

    img->pixels = malloc(img->width * img->height);

    if (img->pixels == NULL)
    {
        free(img);
        SDL_FreeSurface(surface);
        return NULL;
    }

    SDL_LockSurface(surface);

    Uint8 *pixels = (Uint8 *)surface->pixels;

    for (int y = 0; y < img->height; y++)
    {
        for (int x = 0; x < img->width; x++)
        {
            Uint8 *p = pixels + y * surface->pitch + x * 4;

            int gray = (p[0] + p[1] + p[2]) / 3;

            img->pixels[y * img->width + x] =
                (gray < 128) ? 1 : 0;
        }
    }

    SDL_UnlockSurface(surface);

    SDL_FreeSurface(surface);

    return img;
}

void free_image(Image *img)
{
    if (img == NULL)
        return;

    free(img->pixels);
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
