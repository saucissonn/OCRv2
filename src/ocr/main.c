#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>

#include "process_img/image.h"
#include "process_img/transform.h"
#include "useful/utils.h"
#include "process_img/detection.h"
#include "useful/matrix.h"
#include "neural_network/nn.h"
#include "process_img/fs.h"
#include "neural_network/ocr.h"
#include "useful/globals.h"
#include "neural_network/train.h"

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
