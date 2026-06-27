#include "display.h"

#include <SDL2/SDL.h>

void display_image(Image *img)
{
    SDL_Window *window = SDL_CreateWindow(
        "Image",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        img->width,
        img->height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (window == NULL)
        return;

    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == NULL)
    {
        SDL_DestroyWindow(window);
        return;
    }

    SDL_Texture *texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        img->width,
        img->height);

    if (texture == NULL)
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        return;
    }

    Uint32 *buffer = malloc(img->width * img->height * sizeof(Uint32));

    if (buffer == NULL)
    {
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        return;
    }

    for (int y = 0; y < img->height; y++)
    {
        for (int x = 0; x < img->width; x++)
        {
            if (get_pixel(img, x, y))
                buffer[y * img->width + x] = 0x00000000; // noir
            else
                buffer[y * img->width + x] = 0xFFFFFFFF; // blanc
        }
    }

    SDL_UpdateTexture(
        texture,
        NULL,
        buffer,
        img->width * sizeof(Uint32));

    free(buffer);

    int running = 1;

    while (running)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = 0;
        }

        int w, h;
        SDL_GetWindowSize(window, &w, &h);

        SDL_Rect dst = {0, 0, w, h};

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, &dst);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}
