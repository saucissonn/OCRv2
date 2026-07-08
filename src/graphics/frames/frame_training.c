#include "../frame.h"
#include "../gui_elements/rectangle.h"
#include "../gui_elements/text.h"
#include "../gui_elements/button.h"
#include "../gui_elements/text_area.h"
#include "../image.h"
#include "../globals.h"
#include "frames.h"
#include "common_elements.h"
#include "../../ocr/neural_network/ocr.h"
#include "../../ocr/useful/globals_ocr.h"
#include "../../ocr/neural_network/save.h"
#include "../../ocr/neural_network/train.h"
#include "../../ocr/process_img/fs.h"

#include <pthread.h>
#include <time.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

static void call_thread_ocr(void **data);

static Rectangle *init_rectangles() {
    Rectangle *rectangles = NULL;

    Rectangle *left_bar = create_rectangle(0, 0, BAR_W, 1, 0, Gray);
    rectangles = add_rectangle(rectangles, left_bar);

    Rectangle *default_background = create_rectangle(BAR_W, 0, 1 - BAR_W, 1, 0, White);
    rectangles = add_rectangle(rectangles, default_background);

    return rectangles;
}

static Text *init_texts() {
    Text *res = NULL;

    Text *txt1 = create_text("OCR training:", DEFAULT_SPACE_W, DEFAULT_SPACE_W, FONT_SIZE5, White);
    res = add_text(res, txt1);

    Text *txt2 = create_text("Number of input neurons: 784", DEFAULT_SPACE_W, 0.07, FONT_SIZE3, White);
    res = add_text(res, txt2);

    Text *txt3 = create_text("Number of output neurons: 9", DEFAULT_SPACE_W, 0.1, FONT_SIZE3, White);
    txt3->y = txt2->y + (float)txt2->rect->h / DELTA_H;
    txt3->rect->y = txt2->rect->y + txt2->rect->h;
    res = add_text(res, txt3);

	Text *txt4 = create_text("Number of hidden neurons per layer", DEFAULT_SPACE_W, 0.1, FONT_SIZE3, White);
	txt4->y = 0.2 - (float)txt4->rect->h / DELTA_H;
	txt4->rect->y = 0.2 * DELTA_H - (float)txt4->rect->h;
	res = add_text(res, txt4);

    Text *txt5 = create_text("Number of threads", DEFAULT_SPACE_W, 0.1, FONT_SIZE3, White);
    txt5->y = 0.3 - (float)txt5->rect->h / DELTA_H;
    txt5->rect->y = 0.3 * DELTA_H - (float)txt5->rect->h;
    res = add_text(res, txt5);

    Text *txt6 = create_text("Number of images to read (100 char / image)", DEFAULT_SPACE_W, 0.1, FONT_SIZE3, White);
    txt6->y = 0.4 - (float)txt6->rect->h / DELTA_H;
    txt6->rect->y = 0.4 * DELTA_H - (float)txt6->rect->h;
    res = add_text(res, txt6);

    Text *txt7 = create_text("Name of the save file (.ocr will be added)", DEFAULT_SPACE_W, 0.1, FONT_SIZE3, White);
    txt7->y = 0.5 - (float)txt7->rect->h / DELTA_H;
    txt7->rect->y = 0.5 * DELTA_H - (float)txt7->rect->h;
    res = add_text(res, txt7);

    return res;
}

static Button *init_buttons(Frame *frame) {
    Button *res = NULL;

	Button *btn1 = create_options_button(frame);

    res = add_button(res, btn1);

    Text *text2 = create_text("Launch & Save", 0, 0, FONT_SIZE4, White);
    Rectangle *rect2 = create_rectangle(DEFAULT_SPACE_W, 0.85, 0.25, 0.1, DEFAULT_BORDER_W, Blue);
    Button *btn2 = create_button(1, text2, rect2);
    btn2->function = call_thread_ocr;
	btn2->args = malloc(sizeof(void *) * 2);
    btn2->args[0] = frame;
    btn2->args[1] = NULL;

	res = add_button(res, btn2);

    return res;
}

static TextArea *init_text_areas() {
    TextArea *res = NULL;

    for (int i = 0; i < 3; i++)
    {
		Text *text1 = create_text("0", 0, 0, FONT_SIZE3, Black);
		Rectangle *rect1 = create_rectangle(DEFAULT_SPACE_W + 0.1 * i, 0.2, 0.09, 0.05, DEFAULT_BORDER_W, White);
		TextArea *txta1 = create_text_area(text1, rect1);
		txta1->alpha = 0;

		res = add_text_area(res, txta1);
	}

	Text *text2 = create_text("0", 0, 0, FONT_SIZE3, Black);
	Rectangle *rect2 = create_rectangle(DEFAULT_SPACE_W, 0.3, 0.09, 0.05, DEFAULT_BORDER_W, White);
	TextArea *txta2 = create_text_area(text2, rect2);
	txta2->alpha = 0;

    res = add_text_area(res, txta2);

    Text *text3 = create_text("0", 0, 0, FONT_SIZE3, Black);
    Rectangle *rect3 = create_rectangle(DEFAULT_SPACE_W, 0.4, 0.09, 0.05, DEFAULT_BORDER_W, White);
    TextArea *txta3 = create_text_area(text3, rect3);
    txta3->alpha = 0;

	res = add_text_area(res, txta3);

    Text *text4 = create_text("save", 0, 0, FONT_SIZE3, Black);
    Rectangle *rect4 = create_rectangle(DEFAULT_SPACE_W, 0.5, 0.25, 0.05, DEFAULT_BORDER_W, White);
    TextArea *txta4 = create_text_area(text4, rect4);

    res = add_text_area(res, txta4);

    return res;
}

static Cursor *init_cursors() {
	return NULL;
}

static void quit_function(void **data)
{
    Frame *frame = data[0];

	destroy_frame(frame->additional_display_args[0]); // Frame stats
    clear_frame(frame);
}

static void additional_display_function(void **data)
{
    Frame *frame_stats = data[0];

    int c_steps;
    int longest_streak;
    int current_streak;
    double average_guesses;
    double average_certainty;

    pthread_mutex_lock(&mutex_general_ocr);

    if (!global_ocr)
    {
        pthread_mutex_unlock(&mutex_general_ocr);
        display_frame(frame_stats);
        return;
    }

	
	pthread_mutex_lock(&mutex_char);
    c_steps = *global_ocr->c_steps;
    longest_streak = *global_ocr->longest_streak;
    current_streak = *global_ocr->current_streak;

    average_guesses = c_steps
        ? *global_ocr->sum_guesses / (double)c_steps
        : 0;

    average_certainty = c_steps
        ? *global_ocr->sum_certainty / (double)c_steps
        : 0;

	
	pthread_mutex_unlock(&mutex_char);
    pthread_mutex_unlock(&mutex_general_ocr);

    Text *curr = frame_stats->texts->next;

    set_text_int(Renderer, curr, c_steps);
    curr = curr->next;

    set_text_int(Renderer, curr, longest_streak);
    curr = curr->next;

    set_text_int(Renderer, curr, current_streak);
    curr = curr->next;

    set_text_double(Renderer, curr, average_guesses);
    curr = curr->next;

    set_text_double(Renderer, curr, average_certainty);

    display_frame(frame_stats);
}

static Text *init_texts_additional_display()
{
    Text *res = NULL;

	Text *txt1 = create_text("OCR stats:", DEFAULT_SPACE_W + BAR_W, DEFAULT_SPACE_W, FONT_SIZE5, Black);
	res = add_text(res, txt1);

	Text *txt2 = create_text("Number char read: ", DEFAULT_SPACE_W + BAR_W, 0.1, FONT_SIZE3, Black);
	txt2->y = 0.2 - (float)txt2->rect->h / DELTA_H;
	txt2->rect->y = 0.2 * DELTA_H - (float)txt2->rect->h;
	txt2->len_max = 1000;
	res = add_text(res, txt2);

	Text *txt3 = create_text("Longest streak: ", DEFAULT_SPACE_W + BAR_W, 0.1, FONT_SIZE3, Black);
	txt3->y = 0.3 - (float)txt3->rect->h / DELTA_H;
	txt3->rect->y = 0.3 * DELTA_H - (float)txt3->rect->h;
	txt3->len_max = 1000;
	res = add_text(res, txt3);

	Text *txt4 = create_text("Current streak: ", DEFAULT_SPACE_W + BAR_W, 0.1, FONT_SIZE3, Black);
	txt4->y = 0.4 - (float)txt4->rect->h / DELTA_H;
	txt4->rect->y = 0.4 * DELTA_H - (float)txt4->rect->h;
	txt4->len_max = 1000;
	res = add_text(res, txt4);

	Text *txt5 = create_text("Average of guesses: ", DEFAULT_SPACE_W + BAR_W, 0.1, FONT_SIZE3, Black);
	txt5->y = 0.5 - (float)txt5->rect->h / DELTA_H;
	txt5->rect->y = 0.5 * DELTA_H - (float)txt5->rect->h;
	txt5->len_max = 1000;
	res = add_text(res, txt5);

	Text *txt6 = create_text("Average of certainty: ", DEFAULT_SPACE_W + BAR_W, 0.1, FONT_SIZE3, Black);
	txt6->y = 0.6 - (float)txt6->rect->h / DELTA_H;
	txt6->rect->y = 0.6 * DELTA_H - (float)txt6->rect->h;
	txt6->len_max = 1000;
	res = add_text(res, txt6);

    return res;
}


static void init_additional_display(Frame *frame)
{
    frame->additional_display_function = additional_display_function;
    frame->additional_display_args = malloc(sizeof(void *) * 2);

	// Possible to only create texts but it's better if more gui needs to be added later on
	Frame *frame_stats = create_frame(NULL); // No parent = No conflict
	
	frame_stats->texts = init_texts_additional_display();

	frame->additional_display_args[0] = frame_stats;
	frame->additional_display_args[1] = NULL;
}

static void *setup_and_train_ocr(void *data)
{
    void **args = data;

	int nb_layers = *(int *)args[0];
	int *size_layers = args[1];
	int nb_threads = *(int *)args[2];
	int c_steps_local = *(int *)args[3];

	if (nb_threads < 1)
		nb_threads = 1;
	if (nb_threads > 10)
		nb_threads = 10;

	global_ocr = create_ocr(0.01, nb_layers, size_layers);

	pthread_barrier_init(&barrier_char, NULL, nb_threads);

    double learning_coeff = 0.01;

    for (int i = 0; i < c_steps_local; i++)
    {
        size_t nb_file = random_file("ocr/images/training/ribbon");

        train(nb_file, global_ocr, nb_threads);
    }

    save_ocr("saves/save.ocr", global_ocr);

	pthread_mutex_lock(&mutex_general_ocr);

    destroy_ocr(global_ocr);
	global_ocr = NULL;

    free(args[0]);
    free(args[2]);
    free(args[3]);
    free(args);

	pthread_mutex_unlock(&mutex_general_ocr);

	pthread_barrier_destroy(&barrier_char);
}

static void call_thread_ocr(void **data)
{
	Frame *frame = data[0];	

	void **data2 = malloc(sizeof(void *) * 5);

	TextArea *curr = frame->text_areas;
	
	int nb_layers = 2;
    for (int i = 0; i < 3; i++)
    {
        if (atoi(curr->text->text))
			nb_layers += 1;

        curr = curr->next;
    }

	if (nb_layers < 3)
	{
		free(data2);
		printf("No hidden layers\n");
		return;
	}

	int *size_layers = malloc(sizeof(int) * nb_layers);
	size_layers[0] = 28 * 28;

	curr = frame->text_areas;

	int c = 1;
	for (int i = 0; i < 3; i++)
	{
		if (atoi(curr->text->text))
		{
			size_layers[c] = atoi(curr->text->text);
			c += 1;
		}		

		curr = curr->next;
	}

	size_layers[c] = 9;

	int *addr_nb_layers = malloc(sizeof(int));
	*addr_nb_layers = nb_layers;

	int *addr_nb_threads = malloc(sizeof(int));
	*addr_nb_threads = atoi(curr->text->text);
	
	curr = curr->next;

	int *addr_steps = malloc(sizeof(int));
	*addr_steps = atoi(curr->text->text);

	data2[0] = addr_nb_layers;
	data2[1] = size_layers;
	data2[2] = addr_nb_threads;
	data2[3] = addr_steps; // Steps
	data2[4] = NULL;

    pthread_t thread_ocr;

    pthread_create(&thread_ocr, NULL, setup_and_train_ocr, data2);

	pthread_detach(thread_ocr);
}

void init_frame_training(Frame *frame)
{
    frame->rectangles = init_rectangles();
    frame->texts = init_texts();
    frame->buttons = init_buttons(frame);
    frame->text_areas = init_text_areas();
    frame->cursors = init_cursors();

    frame->quit_function = quit_function;
    frame->quit_args = malloc(sizeof(void *) * 2);
    frame->quit_args[0] = frame;
    frame->quit_args[1] = NULL;

	init_additional_display(frame);
}

