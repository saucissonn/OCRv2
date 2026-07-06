#include "matrix.h"

#include <stdio.h>
#include <stdlib.h>

int *coordinates_to_matrix(Image *img, int x0, int y0, int x1, int y1)
{
	if (!img || !img->pixels || x0 < 0 || y0 < 0 ||
		x1 < x0 || y1 < y0 || img->width <= x1 || img->height <= y1)
		return NULL;

	int *out = malloc(sizeof(int) * (x1 - x0 + 1) * (y1 - y0 + 1));

	int w = x1 - x0 + 1;
	int h = y1 - y0 + 1;

	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			out[y * w + x] = img->pixels[(y0 + y) * img->width + (x0 + x)];
		}
	}

	return out;
}

int *matrix_to_28x28(int *matrix, int w, int h)
{
    if (!matrix || w <= 0 || h <= 0)
        return NULL;

    int *out = malloc(28 * 28 * sizeof(int));
    if (!out)
        return NULL;

    for (int y = 0; y < 28; y++)
    {
        int src_y = y * h / 28;

        for (int x = 0; x < 28; x++)
        {
            int src_x = x * w / 28;
            out[y * 28 + x] = matrix[src_y * w + src_x];
        }
    }

    return out;
}

void print_matrix(int *matrix, int w, int h)
{
    if (!matrix || w <= 0 || h <= 0)
        return;

    for (int y = 0; y < 28; y++)
    {
        for (int x = 0; x < 28; x++)
        {
			char symbol = (matrix[y * w + x] == 1) ? '#' : '.';
			printf("%c", symbol);
		}
		printf("\n");
    }
}
