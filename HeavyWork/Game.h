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
#include "Tokens.h"

int game(Window window, Textures tex, Mix_Chunk *recoger, Mix_Chunk *invisi, long long int* score);
bool invencibilidad (float time, int *tiempo_fin_invencibilidad, bool invenc);
int calcPuntos(Tokens tok[],int ntok,float game_time);
int finvelo (float gametime, int ntokens, int tiempo_fin_rap, int tiempo_fin_lent, int velocidad);
void exitScreen(Window window, Textures tex, long long int score);

