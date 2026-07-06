#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "image.h"

Image *rotate_image(Image *src, double angle);

Image *crop_white(Image *src);

double hough_angle(Image *img, double precision);

#endif
