#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_audio.h>
#include "Utilities.h"
#include "Laberinto.h"
#include "Player.h"
#include "Enemies.h"


int game(Window window, Textures tex, player_t* player, player_t* bot, Mix_Chunk *recoger, Mix_Chunk *invisi);
bool invencibilidad (float time, int *tiempo_fin_invencibilidad, bool invenc);
