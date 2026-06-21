#include "frame.h"
#include "rectangle.h"
#include "text.h"
#include "button.h"
#include "text_area.h"
#include "image.h"
#include "globals.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

Frame *create_frame(Frame *parent) {
	Frame *frame = malloc(sizeof(Frame));

	frame->name = NULL;

    frame->image = NULL;
    frame->image_w = 0;
    frame->image_h = 0;

    frame->rectangles = NULL;
    frame->texts = NULL;
    frame->buttons = NULL;
    frame->text_areas = NULL;
    frame->current_text_area = NULL;
	frame->cursors = NULL;
	frame->current_cursor = NULL;

	frame->parentframe = parent;
    frame->subframe = NULL;

	if (parent)
		parent->subframe = frame;

    return frame;
}

void clear_current_states(Frame *frame) { // Important when the user clicks to have one interaction at the time
	frame->current_text_area = NULL;
	frame->current_cursor = NULL;
}

void clear_frame(Frame *frame) {
	free(frame->name);
	frame->name = NULL;

	frame->image = NULL;
    frame->image_w = 0;
    frame->image_h = 0;

	destroy_rectangles(frame->rectangles);
    frame->rectangles = NULL;

	destroy_texts(frame->texts);
    frame->texts = NULL;

	destroy_buttons(frame->buttons);
    frame->buttons = NULL;

	destroy_text_areas(frame->text_areas);
    frame->text_areas = NULL;
    frame->current_text_area = NULL;

	destroy_cursors(frame->cursors);
	frame->cursors = NULL;

	frame->parentframe = NULL;

	clear_current_states(frame);

	destroy_frame(frame->subframe);
    frame->subframe = NULL;
}

void init_frame(Frame *frame) {
	frame->rectangles = init_rectangles();
	frame->texts = init_texts();
	frame->buttons = init_buttons(frame);	
	frame->text_areas = init_text_areas();
	frame->cursors = init_cursors();
}

void init_frame2(Frame *frame) {
    //frame->rectangle = init_rectangle();
    frame->texts = init_texts();
    frame->buttons = init_buttons(frame);
    frame->text_areas = init_text_areas();
}

void destroy_frame(Frame *frame) {
	if (!frame)
		return;
	
	free(frame->name);

	if (frame->image)
		SDL_DestroyTexture(frame->image);

	clear_frame(frame);

	destroy_frame(frame->subframe);

	free(frame);
}

void display_frame(Frame *frame) {
	if (!frame)
		return;

    display_rectangles(Renderer, frame->rectangles, 0);
    display_texts(frame->texts);
    display_buttons(Renderer, frame->buttons);
    display_text_areas(Renderer, frame->text_areas);
    display_blinker(Renderer, frame->current_text_area);
	display_cursors(Renderer, frame->cursors);

	render_image(frame);

	display_frame(frame->subframe);
}
