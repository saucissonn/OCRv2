#include "globals_ocr.h"

pthread_barrier_t barrier_char;
pthread_mutex_t mutex_char = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_SGD = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_general_ocr = PTHREAD_MUTEX_INITIALIZER;

Ocr *global_ocr = NULL;

int SGD_ready = 1;

