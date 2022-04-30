#pragma once
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_audio.h>
#include "Utilities.h"
#include <SDL2/SDL_rect.h>
#include "Utilities.h"

typedef struct
{
    SDL_Rect texture;
} bot_struct;

bot_struct* bot_creator(Vector2i initial_position);
void renderBot(bot_struct* bot, Window window);
void mov_bot (int num_aleat, bot_struct* bot);
