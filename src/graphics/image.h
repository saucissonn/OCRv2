#ifndef IMAGE_H
#define IMAGE_H

#include "frame.h"

int load_image(Frame *frame, const char *path);
void render_image(Frame *frame);

#endif
