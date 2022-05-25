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
#include "Font.h"

int game(Window window, Textures tex, Mix_Chunk *recoger, Mix_Chunk *invisi);
bool invencibilidad (float time, int *tiempo_fin_invencibilidad, bool invenc);
int finvelo (float gametime, int ntokens, int tiempo_fin_rap, int tiempo_fin_lent, int velocidad);
static void renderScoreScreen(Window window, SDL_Texture* text, SDL_Texture* boton_aceptar, font_texture_t tex_filename);
int exitScreen(Window window, Textures tex, long long int score);

