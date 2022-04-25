#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Utilities.h"

typedef struct
{
    bool visited;
    bool south;
    bool north;
    bool east;
    bool west;

} Cell; //Estructura para ver generar un laberinto a partir de celdas

typedef struct
{
    int w,h;
    char *esq;

} M_Lab;

void generarLaberinto(M_Lab m_Lab);
void drawLab(Window window, M_Lab m_Lab, Entity muros[], SDL_Texture* wallTex);
void DebugLab(M_Lab m_Lab);
