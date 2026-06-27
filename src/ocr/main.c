#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "image.h"
#include "transform.h"
#include "display.h"

int main(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("%s", SDL_GetError());
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        SDL_Log("%s", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    Image *img = load_png("images/reference_tilted.png");

    if (img == NULL)
    {
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    Image *rotated = rotate_image(img, -10.9);

    if (rotated == NULL)
    {
        free_image(img);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    Image *cropped = crop_white(rotated);

    if (cropped == NULL)
    {
        free_image(rotated);
        free_image(img);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    display_image(cropped);

    free_image(cropped);
    free_image(rotated);
    free_image(img);

    IMG_Quit();
    SDL_Quit();

    return 0;
}
