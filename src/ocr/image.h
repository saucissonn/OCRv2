#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>

typedef struct
{
    int width;
    int height;
    uint8_t *pixels;    // 0 = blanc, 1 = noir
} Image;

Image *load_png(const char *filename);
void free_image(Image *img);

uint8_t get_pixel(Image *img, int x, int y);
void set_pixel(Image *img, int x, int y, uint8_t value);

#endif
