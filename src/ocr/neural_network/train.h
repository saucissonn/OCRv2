#ifndef TRAIN_H
#define TRAIN_H

#include "layer.h"

#include <stddef.h>

double start_to_loss(int *input, Layer *input_layer, int expected, double *learning_coeff);

double browse(int *input, Layer *input_layer, int expected, double *learning_coeff);
void average_gradients(Layer **outputs, int nb_threads);
void *train_thread(void *arg);
void train(size_t nb_file, Layer *ocr, double *learning_coeff, int nb_threads);

void launch_ocr(char *file, Layer *ocr, double *learning_coeff);

#endif
