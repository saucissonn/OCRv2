#include "gui_elements/text.h"
#include "utils.h"
#include "globals.h"
#include "../ocr/useful/globals_ocr.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <unistd.h>

int collision_rect(int x, int y, SDL_Rect rect)
{
    return (x >= rect.x &&
            x < rect.x + rect.w &&
            y >= rect.y &&
            y < rect.y + rect.h);
}

void init_global_variables()
{
    // Define global variables for graphics
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        return;
    }

    arrowCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    handCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
    textCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        fprintf(stderr, "IMG_Init: %s\n", IMG_GetError());
        return;
    }

    if (TTF_Init() == -1)
    {
        fprintf(stderr, "TTF_Init: %s\n", TTF_GetError());
        return;
    }

    Window = SDL_CreateWindow(
        "Drag & Drop PNG",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIN_W,
        WIN_H,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (!Window)
        return;

    SDL_SetWindowMinimumSize(Window, WINDOW_MIN_W, WINDOW_MIN_H);

    Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_SOFTWARE);

    if (!Renderer)
        return;

    if (!font_init(Renderer, "graphics/DejaVuSans.ttf"))
        fprintf(stderr, "Impossible to load the font\n");

	// Define global variables for the OCR
	NB_THREADS_MAX = sysconf(_SC_NPROCESSORS_ONLN) / 2;
	if (NB_THREADS_MAX < 1)
		NB_THREADS_MAX = 1;
}
