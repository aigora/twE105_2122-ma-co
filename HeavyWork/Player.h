#pragma once
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_audio.h>
#include "Utilities.h"

typedef struct {
    SDL_Texture* player_right;
    SDL_Texture* player_left;
    SDL_Texture* player_up;
    SDL_Texture* player_down;

    SDL_Texture* player_inv_right;
    SDL_Texture* player_inv_left;
    SDL_Texture* player_inv_up;
    SDL_Texture* player_inv_down;

    SDL_Texture* life;
} player_textures_t;

typedef enum {
    MOVEMENT_UP = 0,
    MOVEMENT_DOWN,
    MOVEMENT_RIGHT,
    MOVEMENT_LEFT,
} player_direction_t;

typedef struct {
    SDL_Rect texture;
    Vector2i pos_inicial;
    int num_vidas;
    player_direction_t direction;
    player_textures_t textures;
} player_t;

player_t* newPlayer(Vector2i initial_position, int num_vidas, player_textures_t textures);
void renderPlayer(player_t* player, Window window, bool invisibilidad);
float playerDist(player_t* v1, Bot bot, const Entity* muros, int num_muros);
bool playerKill(player_t* player);
bool invisibility(float time, int tiempo_fin_invisibilidad, bool boton, Mix_Chunk *invisi);
void playerSetDirection(player_t* player, player_direction_t direction);
int ColisionPlayer(const player_t player, Entity entity);
