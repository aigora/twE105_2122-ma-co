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

typedef struct
{
    SDL_Texture* tex;
    SDL_Rect src;//Donde consigo la imagen
    SDL_Rect dst;//Posición y escala en pantalla
    bool collected;

} Tokens_struct;

Tokens* generarToken(SDL_Texture* Textura);
