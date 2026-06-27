#include "transform.h"

#include <stdlib.h>
#include <math.h>

Image *rotate_image(Image *src, double angle)
{
    double rad = angle * M_PI / 180.0;

    double c = cos(rad);
    double s = sin(rad);

    int new_width =
        (int)(fabs(src->width * c) + fabs(src->height * s) + 0.5);

    int new_height =
        (int)(fabs(src->width * s) + fabs(src->height * c) + 0.5);

    Image *dst = malloc(sizeof(Image));

    if (dst == NULL)
        return NULL;

    dst->width = new_width;
    dst->height = new_height;

    dst->pixels = malloc(new_width * new_height);

    if (dst->pixels == NULL)
    {
        free(dst);
        return NULL;
    }

    // fond blanc
    for (int i = 0; i < new_width * new_height; i++)
        dst->pixels[i] = 0;

    double cx_src = src->width / 2.0;
    double cy_src = src->height / 2.0;

    double cx_dst = new_width / 2.0;
    double cy_dst = new_height / 2.0;

    for (int y = 0; y < new_height; y++)
    {
        for (int x = 0; x < new_width; x++)
        {
            double dx = x - cx_dst;
            double dy = y - cy_dst;

            double sx =  c * dx + s * dy + cx_src;
            double sy = -s * dx + c * dy + cy_src;

            int ix = (int)(sx + 0.5);
            int iy = (int)(sy + 0.5);

            if (ix >= 0 &&
                iy >= 0 &&
                ix < src->width &&
                iy < src->height)
            {
                set_pixel(dst,
                          x,
                          y,
                          get_pixel(src, ix, iy));
            }
        }
    }

    return dst;
}

Image *crop_white(Image *src)
{
    int left = src->width;
    int right = -1;
    int top = src->height;
    int bottom = -1;

    for (int y = 0; y < src->height; y++)
    {
        for (int x = 0; x < src->width; x++)
        {
            if (get_pixel(src, x, y))
            {
                if (x < left)
                    left = x;

                if (x > right)
                    right = x;

                if (y < top)
                    top = y;

                if (y > bottom)
                    bottom = y;
            }
        }
    }

    if (right == -1)
        return NULL;

    Image *dst = malloc(sizeof(Image));

    if (dst == NULL)
        return NULL;

    dst->width = right - left + 1;
    dst->height = bottom - top + 1;

    dst->pixels = malloc(dst->width * dst->height);

    if (dst->pixels == NULL)
    {
        free(dst);
        return NULL;
    }

    for (int y = 0; y < dst->height; y++)
    {
        for (int x = 0; x < dst->width; x++)
        {
            set_pixel(dst,
                      x,
                      y,
                      get_pixel(src,
                                x + left,
                                y + top));
        }
    }

    return dst;
}
