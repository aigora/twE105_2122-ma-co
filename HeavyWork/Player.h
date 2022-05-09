#pragma once
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_audio.h>
#include "Utilities.h"
#include "Enemies.h"

#include <SDL2/SDL_rect.h>

#include "Utilities.h"


typedef enum {
    MOVEMENT_UP = 0,
    MOVEMENT_DOWN,
    MOVEMENT_RIGHT,
    MOVEMENT_LEFT,
} player_direction_t;

player_t* newPlayer(Vector2i initial_position, int num_vidas, SDL_Texture* life_texture);

void renderPlayer(player_t* player, Window window);

void movePlayer(player_t* player, const Entity* muros, int num_muros, player_direction_t direction,float delta_time);

float playerDist(player_t* v1, bot_struct* v2, const Entity* muros, int num_muros);

bool playerKill(player_t* player);

int invisibility(float time,int *aux_invisibilidad, int invisibilidad);


