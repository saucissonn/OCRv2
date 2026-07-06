#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>

#include "image.h"
#include "transform.h"
#include "display.h"
#include "utils.h"
#include "detection.h"
#include "matrix.h"
#include "nn.h"
#include "fs.h"
#include "ocr.h"
#include "globals.h"
#include "train.h"

int main(void)
{
	srand(time(NULL));

    double learning_coeff = 0.01;
    Ocr *ocr = init_ocr();

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

	for (int i = 0; i < 10; i++)
	{
		size_t nb_file = random_file("images/training/ribbon");

		train(nb_file, ocr->nn, &learning_coeff, 8);
	}

	destroy_ocr(ocr);

	pthread_barrier_destroy(&barrier_char);

    IMG_Quit();
    SDL_Quit();

	printf("GOOD ENDING\n");
    return 0;
}
