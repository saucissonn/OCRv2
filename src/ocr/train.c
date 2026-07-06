#include "train.h"
#include "globals.h"
#include "image.h"
#include "matrix.h"
#include "nn.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


double start_to_loss(int *input, Layer *input_layer, int expected, double *learning_coeff)
{
    if (!input)
    {
        printf("no input\n");
        return 0;
    }

    put_in_input(input, 28, 28, input_layer);

    forward(input_layer);

    Layer *output_layer = get_output_layer(input_layer);

    soft_max(output_layer);

    double p = output_layer->output[expected - 1];

    if (p < 1e-8) p = 1e-8;
    double loss = -log(p);
    int idx_max_output = index_max_output(output_layer);
    int result = 0;

    if (idx_max_output == expected - 1)
        result = 1;

    pthread_mutex_lock(&mutex_char);
    printf("loss: %f, value: %f, steps: %d, "
            "expected: %d, get: %d, result: %d, lr: %f\n",
            loss, output_layer->output[expected - 1], c_steps,
            expected - 1, idx_max_output, result, *learning_coeff);

    c_steps++;
    pthread_mutex_unlock(&mutex_char);

    //print_outputs(output_layer);

    for (int i = 0; i < output_layer->current_size; i++)
        output_layer->delta[i] = output_layer->output[i];

    output_layer->delta[expected - 1] -= 1.0; //cross entropy

    return loss;
}

double browse(int *input, Layer *input_layer, int expected, double *learning_coeff)
{
    double loss = start_to_loss(input, input_layer, expected, learning_coeff);

    Layer *output_layer = get_output_layer(input_layer);
    backward(output_layer);

    compute_gradients(output_layer);

    update_SGD(output_layer, *learning_coeff);

    update_learning_coeff(learning_coeff);

    return loss;
}

void average_gradients(Layer **outputs, int nb_threads)
{
    Layer *currs[nb_threads];

    for (int th = 0; th < nb_threads; th++)
        currs[th] = outputs[th];

    while (currs[0] && currs[0]->weights)
    {
        int cs = currs[0]->current_size;
        int ps = currs[0]->previous_size;

        for (int i = 0; i < cs; i++)
        {
            double sum_bias = 0.0;

            for (int th = 0; th < nb_threads; th++)
                sum_bias += currs[th]->grad_biases[i];

            currs[0]->grad_biases[i] =
                sum_bias / nb_threads;

            for (int j = 0; j < ps; j++)
            {
                int idx = i * ps + j;
                double sum = 0.0;

                for (int th = 0; th < nb_threads; th++)
                    sum += currs[th]->grad_weights[idx];

                currs[0]->grad_weights[idx] =
                    sum / nb_threads;
            }
        }

        for (int th = 0; th < nb_threads; th++)
            currs[th] = currs[th]->prev;
    }
}

void *train_thread(void *arg)
{
    void **args = arg;

    Image *img = args[0];
    Layer *ocr = args[1];
    double *learning_coeff = args[2];
    int *ans_list = args[3];

    int start = *(int *)args[4];
    int end = *(int *)args[5];

    int nb_threads = *(int *)args[6];

    Layer **outputs = args[7];

    for (int i = start; i < end; i++)
    {
        int expected = ans_list[i];

        int *square = coordinates_to_matrix(img, 28 * i, 0, 28 * (i + 1) - 1, 27);

        start_to_loss(square, ocr, expected, learning_coeff);

        Layer *output = get_output_layer(ocr);

        backward(output);
        compute_gradients(output);

        int ret = pthread_barrier_wait(&barrier_char); // Wait for every threads to read one char

        if (ret == PTHREAD_BARRIER_SERIAL_THREAD) // One thread updates shared biases and weights
        {
            average_gradients(outputs, nb_threads);

            update_SGD(outputs[0], *learning_coeff);

            update_learning_coeff(learning_coeff);
        }

        pthread_barrier_wait(&barrier_char); // Wait for the SGD update

        free(square);
    }

    return NULL;
}

void train(size_t nb_file, Layer *ocr, double *learning_coeff, int nb_threads)
{
    if (nb_threads < 1)
        nb_threads = 1;

    if (nb_threads > 10)
        nb_threads = 10;

    char file_img_name[255];
    char file_ans_name[255];

    sprintf(file_img_name, "images/training/ribbon/%ld.png", nb_file);
    sprintf(file_ans_name, "images/training/ribbon_ans/%ld.txt", nb_file);

    FILE *ans = fopen(file_ans_name, "r");

    if (!ans)
    {
        perror(file_ans_name);
        return;
    }

    fseek(ans, 0, SEEK_END);
    int file_size = ftell(ans);
    rewind(ans);

    int *ans_list = malloc(file_size * sizeof(int));

    for (int i = 0; i < file_size; i++)
        ans_list[i] = fgetc(ans) - '0';

    Image *img = load_png(file_img_name);

    pthread_t threads[nb_threads];

    void *args[nb_threads][8];

    int starts[nb_threads];
    int ends[nb_threads];

    Layer *ocrs[nb_threads];
    Layer *outputs[nb_threads];

    int step = file_size / nb_threads;

    pthread_barrier_init(&barrier_char, NULL, nb_threads);

    for (int th = 0; th < nb_threads; th++)
    {
        ocrs[th] = create_layer_clone(ocr);
        outputs[th] = get_output_layer(ocrs[th]);
    }

    int offset = 0;

    for (int th = 0; th < nb_threads; th++)
    {
        starts[th] = offset;
        ends[th] = offset + step;

        offset = ends[th];

        args[th][0] = img;
        args[th][1] = ocrs[th];
        args[th][2] = learning_coeff;
        args[th][3] = ans_list;
        args[th][4] = &starts[th];
        args[th][5] = &ends[th];
        args[th][6] = &nb_threads;
        args[th][7] = outputs;

        pthread_create(&threads[th], NULL, train_thread, args[th]);
    }

    for (int th = 0; th < nb_threads; th++)
        pthread_join(threads[th], NULL);

    for (int th = 0; th < nb_threads; th++)
        free_layer_clone(ocrs[th]);

    pthread_barrier_destroy(&barrier_char);

    free(ans_list);
    free_image(img);

    fclose(ans);
}

void launch_ocr(char *file, Layer *ocr, double *learning_coeff)
{
    Image *img = load_png(file);
    process_image(img);

    for (int i = 0; i < 81; i++)
    {
        if (img->valid_squares[i])
        {
            put_in_input(img->squares[i], 28, 28, ocr);
            forward(ocr);

            Layer *output_layer = get_output_layer(ocr);
            soft_max(output_layer);

            int idx_max_output = index_max_output(output_layer);
        }
    }

    free_image(img);
}

