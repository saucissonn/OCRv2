#ifndef APP_H
#define APP_H

#include <SDL2/SDL.h>
#include "gui_elements/rectangle.h"
#include "gui_elements/text.h"
#include "gui_elements/text_area.h"
#include "gui_elements/cursor.h"

typedef struct Button Button;
typedef struct Cursor Cursor;

typedef struct Frame {
	char *name;

    SDL_Texture *image;
    int image_w;
    int image_h;

	Rectangle *rectangles;
	Text *texts;
	Button *buttons;
	TextArea *text_areas;
	TextArea *current_text_area;
	Cursor *cursors;
	Cursor *current_cursor;

	struct Frame *parentframe;
	struct Frame *subframe;
} Frame;


Frame *create_frame(Frame *parent);

void clear_current_states(Frame *frame);
void clear_frame(Frame *frame);

void destroy_frame(Frame *frame);

void display_frame(Frame *frame);

#endif
