#include "ocr.h"

#include <stdlib.h>
#include <stdio.h>

Ocr *create_ocr(double learning_coeff, int nb_hidden_neurons, int nb_hidden_layers)
{
	Ocr *res = malloc(sizeof(Ocr));
	
	res->c_steps = 0;
	res->learning_coeff = learning_coeff;

    res->nb_input_neurons = 28 * 28;
    res->nb_hidden_neurons = nb_hidden_neurons;
    res->nb_output_neurons = 9;

    res->nb_hidden_layers = nb_hidden_layers;

	Layer *nn = create_layer("input", res->nb_input_neurons, res->nb_input_neurons);
	Layer *curr = create_layer("hidden", res->nb_input_neurons, res->nb_hidden_neurons);
	nn->next = curr;
	curr->prev = nn;	

	for (int i = 1; i < res->nb_hidden_layers; i++)
	{
		Layer *temp = create_layer("hidden", res->nb_hidden_neurons, res->nb_hidden_neurons);
		curr->next = temp;
		temp->prev = curr;
		
		curr = temp;
	}

	Layer *temp2 = create_layer("output", res->nb_hidden_neurons, res->nb_output_neurons);
    curr->next = temp2;
    temp2->prev = curr;

    res->nn = nn;

	return res;
}

Ocr *init_ocr()
{
	Ocr *res = create_ocr(0.01, 256, 2);

	return res;
}

void destroy_ocr(Ocr *ocr)
{
	if (!ocr) return;

	free_layers(ocr->nn);
	free(ocr);
}
