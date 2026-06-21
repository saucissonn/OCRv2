#include "text.h"
#include "globals.h"

#include <SDL2/SDL_ttf.h>
#include <string.h>

static SDL_Renderer *g_renderer = NULL;
static TTF_Font *g_font = NULL;

int font_init(SDL_Renderer *renderer, const char *font_file)
{
    g_renderer = renderer;

    g_font = TTF_OpenFont(font_file, 24);

    return g_font != NULL;
}

Text *create_text(char *text, float x, float y, float size, SDL_Color color) {
	Text *res = malloc(sizeof(Text));

	res->text = strdup(text);
	res->color = color;
	res->x = x;
	res->y = y;
	res->size = size;

	TTF_Font *font = TTF_OpenFont("DejaVuSans.ttf", size * DELTA);
	res->font = font;

	if (!*text) printf("HERE\n");

    SDL_Surface *surface = TTF_RenderUTF8_Blended(font, text, color);
	if (!surface) {
		fprintf(stderr, "TTF_RenderUTF8_Blended: %s\n", TTF_GetError());
		return NULL;
	}

    SDL_Texture *texture = SDL_CreateTextureFromSurface(g_renderer, surface);
	res->texture = texture;	

	SDL_Rect *rect = malloc(sizeof(SDL_Rect));
    rect->x = x * DELTA_W;
    rect->y = y * DELTA_H;
    rect->w = surface->w;
    rect->h = surface->h;

	res->rect = rect;

	SDL_FreeSurface(surface);

	res->next = NULL;

	return res;
}

Text *copy_text(Text *text) {
    if (!text) {
        return NULL;
    }

    return create_text(
        text->text,
        text->x,
        text->y,
        text->size,
        text->color
    );
}

Text *add_text(Text *texts, Text *text) {
	if (!texts) return text;

	Text *curr = texts;

	while (curr->next) {
		curr = curr->next;
	}

	curr->next = text;

	return texts;
}

Text *init_texts() {
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

void display_texts(Text *texts) {
	Text *curr = texts;

	while (curr) {
		SDL_RenderCopy(
			g_renderer,
			curr->texture,
			NULL,
			curr->rect);
	
		curr = curr->next;
	}
}

void destroy_texts(Text *texts) {
    Text *curr = texts;

    while (curr) {
		Text *temp = curr;
		
		free(curr->text);
		free(curr->rect);
		TTF_CloseFont(curr->font);
		SDL_DestroyTexture(curr->texture);
		
		curr = curr->next;
		free(temp);
	}
}

void destroy_font() {
    if (g_font)
        TTF_CloseFont(g_font);
}

void modify_text(SDL_Renderer *renderer, Text *text, char *content) {
    if (!content || !text || !renderer) return;

    SDL_Surface* surface = NULL;
    SDL_Texture* newTexture = NULL;

    if (*content) {
        surface = TTF_RenderUTF8_Blended(text->font, content, text->color);
		newTexture = SDL_CreateTextureFromSurface(renderer, surface);
    }

	int w = 0;
	if (surface)
		w = surface->w;

    if (strlen(content) > 8) {
            SDL_FreeSurface(surface);
            SDL_DestroyTexture(newTexture);
    }
    else {
            free(text->text);
			SDL_DestroyTexture(text->texture);

            text->text = strdup(content);
			text->texture = newTexture;

	        if (surface)
				SDL_FreeSurface(surface);

			text->rect->w = w;
    }
}

void add_char(SDL_Renderer *renderer, Text *text, char c) {
    int len = strlen(text->text);
    char buff[len + 2];

    strcpy(buff, text->text);
    buff[len] = c;
    buff[len + 1] = '\0';

    modify_text(renderer, text, buff);
}

void delete_char(SDL_Renderer *renderer, Text *text) {
    int len = strlen(text->text);
    if (!len) return;
	
	char buff[len];

	strncpy(buff, text->text, len - 1);
	buff[len - 1] = '\0';

	modify_text(renderer, text, buff);
}

