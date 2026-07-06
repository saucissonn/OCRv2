#ifndef OCR_H
#define OCR_H

#include "layer.h"

typedef struct Ocr
{
    int c_steps; // How many steps of training, default 0
    double learning_coeff;

    int nb_input_neurons; // Default 28 * 28
    int nb_hidden_neurons; // Default 64
    int nb_output_neurons; // Default 9

    int nb_hidden_layers; // Default 2

    Layer *nn;
} Ocr;

Ocr *create_ocr(double learning_coeff, int nb_hidden_neurons, int nb_hidden_layers);
Ocr *init_ocr();

void destroy_ocr(Ocr *ocr);

#endif
