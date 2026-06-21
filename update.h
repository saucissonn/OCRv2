#ifndef UPDATE_H
#define UPDATE_H

#include "globals.h"
#include "rectangle.h"
#include "text.h"
#include "button.h"
#include "text_area.h"
#include "cursor.h"
#include "frame.h"

void update_mouse();

void resize_rectangles(Rectangle *rectangles);
void update_rectangles(Frame *frame);

void resize_texts(SDL_Renderer *renderer, Text *texts);
void update_texts(Text *texts);

void resize_buttons(SDL_Renderer *renderer, Button *buttons);
void update_button(Button *button, Frame *frame);

void resize_text_areas(SDL_Renderer *renderer, TextArea *text_areas);
void update_text_areas(TextArea *text_areas);

void resize_cursors(SDL_Renderer *renderer, Cursor *cursors);
void update_cursors(Cursor *cursors, Frame *frame);

void resize_image();
void update_image();

void resize_all(Frame *frame);
void update_all(Frame *frame);

#endif
