#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "Utilities.h"
#include "Player.h"
#include "Enemies.h"

typedef struct
{
    bool visited;
    bool south;
    bool north;
    bool east;
    bool west;

} Cell; //Estructura para generar un laberinto a partir de celdas

typedef struct
{
    int w,h;
    char *esq;

} M_Lab;

void generarLaberinto(M_Lab m_Lab);
void drawLab(Window window, M_Lab m_Lab, Entity muros[], SDL_Texture* wallTex);
void DebugLab(M_Lab m_Lab);
void movLab(Entity muros[], int nmuros, key_buttons k, player_t player, Bot bot[],Entity Tok[], int ntokens, int nbots, bool boton, float delta_time);
void generarBots(M_Lab m_Lab, Bot bots[], int nbots, Textures tex);
void renderLab(Window window, Entity muros[], int nmuros);


