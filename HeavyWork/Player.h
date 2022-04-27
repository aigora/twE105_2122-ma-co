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

typedef struct {
    SDL_Rect texture;
} player_t;

player_t* newPlayer(Vector2i initial_position);

void renderPlayer(player_t* player, Window window);

float modist();

