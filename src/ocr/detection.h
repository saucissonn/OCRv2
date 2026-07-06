#ifndef DETECTION_H
#define DETECTION_H

#include "image.h"

int *detect_lines(Image *img, float size, int direction);
int *detect_squares(Image *img, float size_line);

int **get_squares_sudoku(Image *img);
int *get_valid_squares(int **squares);

#endif
