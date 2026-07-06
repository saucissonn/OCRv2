#include "globals.h"

pthread_barrier_t barrier_char;
pthread_mutex_t mutex_char = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_SGD = PTHREAD_MUTEX_INITIALIZER;

int SGD_ready = 1;

int c_steps = 0;
