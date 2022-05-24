#ifndef FONT_H
#define FONT_H
#include "Utilities.h"

#include <SDL2/SDL.h>

typedef struct {
    SDL_Texture* texture;
    int w;
    int h;
} font_texture_t;

font_texture_t newText(const char* text, SDL_Color colour, int font_size, Window window);

#endif
