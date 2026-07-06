#ifndef SAVE_H
#define SAVE_H

#include "nn.h"

void save_ocr(const char *path, Layer *ocr);
Layer *load_ocr(const char *path);

#endif
