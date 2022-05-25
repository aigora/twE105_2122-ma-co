#include "Font.h"
#include "Utilities.h"

#include <SDL2/SDL_ttf.h>

font_texture_t newText(const char* text, SDL_Color colour, int font_size, Window window) {
    int w;
    int h;
    TTF_Font* font = TTF_OpenFont("resources/fonts/Roboto-Bold.ttf", font_size);
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, text, colour);
    TTF_SizeText(font, text, &w, &h);
    font_texture_t font_texture = {
        .texture=SDL_CreateTextureFromSurface(window.renderer, surfaceMessage),
        .w=w,
        .h=h,
    };
    return font_texture;
}

