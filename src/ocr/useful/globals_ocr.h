#ifndef GLOBALS_OCR_H
#define GLOBALS_OCR_H

#include <pthread.h>

#include "../neural_network/ocr.h"

extern pthread_barrier_t barrier_char;
extern pthread_mutex_t mutex_char;
extern pthread_mutex_t mutex_SGD;
extern pthread_mutex_t mutex_general_ocr;

extern Ocr *global_ocr;

extern int SGD_ready;

#endif
