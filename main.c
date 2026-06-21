#include "frame.h"
#include "events.h"
#include "update.h"
#include "utils.h"
#include "globals.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

int main(void)
{
	init_global_variables();

	Frame *frame = create_frame(NULL);
	init_frame(frame);

    if (!frame)
        return 1;

    SDL_Event event;

    while (running)
    {		
		handle_events(event, frame);

        SDL_SetRenderDrawColor(Renderer, 30, 30, 30, 255);

        SDL_RenderClear(Renderer);
	
		display_frame(frame);

		update_all(frame);
		update_mouse();
	
		SDL_RenderPresent(Renderer);
	}

	SDL_FreeCursor(handCursor);
	SDL_FreeCursor(arrowCursor);
	SDL_FreeCursor(textCursor);

	destroy_font();
    destroy_frame(frame);

    SDL_DestroyRenderer(Renderer);
    SDL_DestroyWindow(Window);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}
