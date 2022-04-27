#pragma once

#include <SDL2/SDL_rect.h>

#include "Utilities.h"

typedef struct {
    SDL_Rect texture;
} player_t;

player_t* newPlayer(Vector2i initial_position);

void renderPlayer(player_t* player, Window window);
