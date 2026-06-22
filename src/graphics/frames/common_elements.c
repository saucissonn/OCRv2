#include "../frame.h"
#include "../gui_elements/rectangle.h"
#include "../gui_elements/text.h"
#include "../gui_elements/button.h"
#include "../gui_elements/text_area.h"
#include "../image.h"
#include "../globals.h"
#include "common_elements.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

static void private_init_frame_ocr(void **data) {
    printf("Swapped\n");
    Frame *frame = data[0];

    clear_frame(frame);
    init_frame_ocr(frame);	
}

static void private_init_frame_training(void **data) {
    printf("Swapped\n");
    Frame *frame = data[0];

    clear_frame(frame);
    init_frame_training(frame);     
}

static void private_init_frame_tutorial(void **data) {
    printf("Swapped\n");
    Frame *frame = data[0];

    clear_frame(frame);
    init_frame_tutorial(frame);  
}

static void private_init_frame_credits(void **data) {
    printf("Swapped\n");
    Frame *frame = data[0];

    clear_frame(frame);
    init_frame_credits(frame);
}

void option_button_function(void **data) {
    printf("SUB1\n");
    Button *button = data[0];
    Frame *frame = data[1];

    Frame *frame2 = create_frame(frame);
    Button *sub_buttons = create_submenu(4, button);

    Button *curr = sub_buttons;
    
	modify_text(Renderer, curr->text, "OCR");
    center_text(curr->text, curr->rect);

    curr->function = private_init_frame_ocr;
	curr->args = malloc(sizeof(void *) * 2);
	curr->args[0] = frame;
	curr->args[1] = NULL;

	curr = curr->next;

    modify_text(Renderer, curr->text, "TRAINING");
    center_text(curr->text, curr->rect);

    curr->function = private_init_frame_training;
    curr->args = malloc(sizeof(void *) * 2);
    curr->args[0] = frame;
    curr->args[1] = NULL;

	curr = curr->next;

    modify_text(Renderer, curr->text, "TUTORIAL");
    center_text(curr->text, curr->rect);
    
    curr->function = private_init_frame_tutorial;
    curr->args = malloc(sizeof(void *) * 2);
    curr->args[0] = frame;
    curr->args[1] = NULL;

	curr = curr->next;

    modify_text(Renderer, curr->text, "CREDITS");
    center_text(curr->text, curr->rect);

    curr->function = private_init_frame_credits;
    curr->args = malloc(sizeof(void *) * 2);
    curr->args[0] = frame;
    curr->args[1] = NULL;
    
	curr = curr->next;

    frame2->buttons = sub_buttons;
}

Button *create_options_button(Frame *frame) {
    Text *text = create_text("OPTIONS", 0, 0, FONT_SIZE3, White);
    Rectangle *rect = create_rectangle(0.25, 0, 0.1, 0.05, DEFAULT_BORDER_W, Green);
    Button *btn = create_button(1, text, rect);
    btn->function = option_button_function;
    btn->args = malloc(sizeof(void *) * 3);
    btn->args[0] = btn;
    btn->args[1] = frame;
    btn->args[2] = NULL;

	return btn;
}
