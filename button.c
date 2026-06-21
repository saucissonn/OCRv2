#include "button.h"
#include "text.h"
#include "rectangle.h"
#include "utils.h"
#include "frame.h"
#include "globals.h"

#include <SDL2/SDL_ttf.h>
#include <string.h>

Button *create_submenu(int nb, Button *button);

void center_text(Text *text, Rectangle *rectangle) {
    if (!text || !rectangle) return;

    float dx = (rectangle->w - (text->rect->w) / (float)DELTA_W) / 2;
    float dy = (rectangle->h - (text->rect->h) / (float)DELTA_H) / 2;

    text->x = rectangle->x + dx;
    text->y = rectangle->y + dy;
    text->rect->x = text->x * DELTA_W;
    text->rect->y = text->y * DELTA_H;
}

Button *create_button(int interactible, Text *text, Rectangle *rect) {
	Button *res = malloc(sizeof(Button));

	res->interactible = interactible;
	res->text = text;
	res->rect = rect;

	center_text(res->text, res->rect);
	
	res->function = NULL;
	res->args = NULL;

	res->next = NULL;

	return res;
}

Button *copy_button(Button *button) { // Copy everything from a button except its actions
    if (!button) {
        return NULL;
    }

    Button *res = create_button(
        button->interactible,
        copy_text(button->text),
        copy_rectangle(button->rect)
    );

    res->function = NULL;
    res->args = NULL;

    return res;
}

Button *add_button(Button *buttons, Button *button) {
	if (!buttons) return button;

	Button *curr = buttons;

	while (curr->next) {
		curr = curr->next;
	}
	
	curr->next = button;

	return buttons;
}

// START button functions section

void button_funtion1(void **data) {
	printf("I'm the button function\n");
}

void button_function_sub1(void **data) {
	printf("SUB1\n");
	Button *button = data[0];
	Frame *frame = data[1];

	Frame *frame2 = create_frame(frame);
	Button *sub_buttons = create_submenu(40, button);
	
	Button *curr = sub_buttons;
	modify_text(Renderer, curr->text, "OCR");
	center_text(curr->text, curr->rect);
	curr = curr->next;

    modify_text(Renderer, curr->text, "TRAINING");                                                                           
    center_text(curr->text, curr->rect);                                                                                
    curr = curr->next;

    modify_text(Renderer, curr->text, "TUTORIAL");                                                                           
    center_text(curr->text, curr->rect);                                                                                
    curr = curr->next;

    modify_text(Renderer, curr->text, "CREDITS");                                                                           
    center_text(curr->text, curr->rect);                                                                                
    curr = curr->next;
		
	frame2->buttons = sub_buttons;
}

void button_swap1(void **data) {
	printf("Swapped\n");
	Frame *frame = data[0];
	
	clear_frame(frame);
	init_frame2(frame);
}

// END button functions section

Button *init_buttons(Frame *frame) {
	Button *res = NULL;

	Text *text1 = create_text("See preprocessed image", 0, 0, FONT_SIZE4, White);
	Rectangle *rect1 = create_rectangle(DEFAULT_SPACE_W, 0.35, 0.3, 0.08, DEFAULT_BORDER_W, Blue);
	Button *btn1 = create_button(1, text1, rect1);
	btn1->function = button_funtion1;

	res = add_button(res, btn1);

    Text *text2 = create_text("MENU", 0, 0, FONT_SIZE3, White);
    Rectangle *rect2 = create_rectangle(0.25, 0, 0.1, 0.05, DEFAULT_BORDER_W, Green);
    Button *btn2 = create_button(1, text2, rect2);
    btn2->function = btn2->function = (void (*)(void **))button_function_sub1;
	btn2->args = malloc(sizeof(void *) * 3);
	btn2->args[0] = btn2;
	btn2->args[1] = frame;
	btn2->args[2] = NULL;

	res = add_button(res, btn2);

    Text *text_swap = create_text("swap", 0, 0, 0.03, White);
    Rectangle *rect_swap = create_rectangle(0.45, 0, 0.1, 0.05, DEFAULT_BORDER_W, Green);
    Button *btn_swap = create_button(1, text_swap, rect_swap);
    btn_swap->function = btn_swap->function = (void (*)(void **))button_swap1;
    btn_swap->args = malloc(sizeof(void *) * 2);
    btn_swap->args[0] = frame;
    btn_swap->args[1] = NULL;

	res = add_button(res, btn_swap);

	return res;
}

void display_buttons(SDL_Renderer *renderer, Button *buttons) {
	Button *curr = buttons;

	while (curr) {
		display_rectangles(renderer, curr->rect, 1);
		display_texts(curr->text);
		curr = curr->next;
	}
}

void destroy_buttons(Button *buttons) {
    Button *curr = buttons;

    while (curr) {
		Button *temp = curr;
		destroy_texts(curr->text);
		destroy_rectangles(curr->rect);
		free(curr->args);
		curr = curr->next;
		free(temp);	
	}
}

Button *collision_buttons(int x, int y, Button *buttons) {
    Button *curr = buttons;

    while (curr) {
		if (collision_rect(x, y, *curr->rect->outline)) {
			return curr;
		}
		curr = curr->next;
	}

	return NULL;
}

Button *create_submenu(int nb, Button *button) { // Create nb copies of a button from top to bottom
	Button *res = NULL;	

	for (int i = 1; i <= nb; i++) {
		Button *copy = copy_button(button);
		
		shift_down_rectangle(i, copy->rect);
		center_text(copy->text, copy->rect);

		res = add_button(res, copy);
	}

	return res;
}
