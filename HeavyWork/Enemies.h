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
#include "Player.h"



bot_struct* bot_creator(Vector2i initial_position,SDL_Texture* bot_texture);
void renderBot(bot_struct* bot, Window window);
void mov_bot (int num_aleat, player_t* v1, bot_struct* v2, const Entity* muros, int num_muros,float delta_time);
void perseguir(player_t* v1, bot_struct* v2, const Entity* muros, int num_muros,float delta_time, int invisibilidad);
