#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> //Preguntar si su uso est� permitido
#include <time.h> //Preguntar si su uso est� permitido
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_audio.h>
#include "Utilities.h"
#include "Player.h"

typedef struct
{
    SDL_Texture* tex;
    SDL_Rect src;//Donde consigo la imagen
    SDL_Rect dst;//Posición y escala en pantalla
    bool collected;

} Tokens_struct;

void TokensCreator(Tokens Token[],Textures tex, Vector2f v[], int type, int ntokens);
void renderToken(Tokens Token[], Window window, int ntokens);
void catchToken(Tokens Token[], int ntokens, player_t* player, Textures tex, Mix_Chunk *efecto, float gametime, int tiempo_fin_rap[1], int tiempo_fin_lent[1], int *velocidad, long long int *puntos);
