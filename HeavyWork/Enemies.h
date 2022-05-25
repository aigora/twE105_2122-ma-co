#pragma once
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_audio.h>
#include "Utilities.h"
#include "Player.h"
#include <SDL2/SDL_rect.h>

void bot_creator(Bot bot[], Vector2i p_initial[], SDL_Texture* tex, int nbots);
void renderBot(Bot bots[], Window window, int nbots);
void mov_bot (Bot* bot, const Entity* muros, int num_muros, float delta_time);
void perseguir(player_t* v1, Bot* bot, const Entity* muros, int num_muros,float delta_time, int invisibilidad);
