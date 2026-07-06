#ifndef GLOBALS_H
#define GLOBALS_H

#include <pthread.h>

extern pthread_barrier_t barrier_char;
extern pthread_mutex_t mutex_char;
extern pthread_mutex_t mutex_SGD;

extern int SGD_ready;

extern int c_steps;

#endif
