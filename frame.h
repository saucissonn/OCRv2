#ifndef APP_H
#define APP_H

#include <SDL2/SDL.h>
#include "rectangle.h"
#include "text.h"
#include "text_area.h"
#include "cursor.h"

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

void init_frame(Frame *frame);
void init_frame2(Frame *frame);

void destroy_frame(Frame *frame);

void display_frame(Frame *frame);

#endif
