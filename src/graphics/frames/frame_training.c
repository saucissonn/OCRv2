#include "../frame.h"
#include "../gui_elements/rectangle.h"
#include "../gui_elements/text.h"
#include "../gui_elements/button.h"
#include "../gui_elements/text_area.h"
#include "../image.h"
#include "../globals.h"
#include "frames.h"
#include "common_elements.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

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

    Text *txt1 = create_text(
        "OCR training:",
        DEFAULT_SPACE_W,
        DEFAULT_SPACE_W,
        FONT_SIZE4,
        White
    );

    res = add_text(res, txt1);

    return res;
}

static Button *init_buttons(Frame *frame) {
    Button *res = NULL;

	Button *btn = create_options_button(frame);

    res = add_button(res, btn);

    return res;
}

static TextArea *init_text_areas() {
	return NULL;
}

static Cursor *init_cursors() {
	return NULL;
}

void init_frame_training(Frame *frame) {
	frame->rectangles = init_rectangles();
	frame->texts = init_texts();
	frame->buttons = init_buttons(frame);	
	frame->text_areas = init_text_areas();
	frame->cursors = init_cursors();
}
