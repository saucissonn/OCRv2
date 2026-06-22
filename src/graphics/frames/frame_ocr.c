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
        "OCR parameters:",
        DEFAULT_SPACE_W,
        DEFAULT_SPACE_W,
        FONT_SIZE4,
        White
    );

    res = add_text(res, txt1);

    Text *txt2 = create_text(
        "Insert an image here",
        0.5,
        0.5,
        FONT_SIZE5,
        Black
    );

    res = add_text(res, txt2);

    return res;
}


static void button_funtion1(void **data) {
    printf("I'm the button function:\nSee preprocessed image");
}

static void button_swap1(void **data) {
    printf("Swapped\n");
    Frame *frame = data[0];

    clear_frame(frame);
    init_frame_training(frame);
}

static Button *init_buttons(Frame *frame) {
    Button *res = NULL;

    Text *text1 = create_text("See preprocessed image", 0, 0, FONT_SIZE4, White);
    Rectangle *rect1 = create_rectangle(DEFAULT_SPACE_W, 0.35, 0.3, 0.08, DEFAULT_BORDER_W, Blue);
    Button *btn1 = create_button(1, text1, rect1);
    btn1->function = button_funtion1;

    res = add_button(res, btn1);

	Button *btn2 = create_options_button(frame);

    res = add_button(res, btn2);

    return res;
}

static TextArea *init_text_areas() {
    TextArea *res = NULL;

    Text *text1 = create_text("Area1", 0, 0, 0.05, Black);
    Rectangle *rect1 = create_rectangle(DEFAULT_SPACE_W, 0.2, 0.3, 0.1, DEFAULT_BORDER_W, White);

    TextArea *txta1 = create_text_area(text1, rect1);

    res = add_text_area(res, txta1);

    return res;
}

static Cursor *init_cursors() {
    Cursor *res = NULL;

    Cursor *crs1 = create_cursor(DEFAULT_SPACE_W, 0.8, 0.15, 0.03, 0, 100.);

    res = add_cursor(res, crs1);

    return res;
}

void init_frame_ocr(Frame *frame) {
	frame->rectangles = init_rectangles();
	frame->texts = init_texts();
	frame->buttons = init_buttons(frame);	
	frame->text_areas = init_text_areas();
	frame->cursors = init_cursors();
}
