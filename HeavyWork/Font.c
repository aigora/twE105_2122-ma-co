#include "Font.h"

#include <SDL2/SDL_ttf.h>

SDL_Texture* newText(const char* text, Window window) {
    TTF_Font* font = TTF_OpenFont("resources/fonts/Roboto-Bold.ttf", 24);
    SDL_Color white = { 255, 255, 255 };
    SDL_Surface* surfaceMessage =
        TTF_RenderText_Solid(font, text, white);
    return SDL_CreateTextureFromSurface(window.renderer, surfaceMessage);
}

