#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "Utilities.h"
#include "Player.h"
#include "Enemies.h"
#include "Tokens.h"

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
void drawLab(Window window, M_Lab m_Lab, Entity muros[], Textures tex);
void DebugLab(M_Lab m_Lab);
void movLab(Entity muros[], Entity *salida, int nmuros, key_buttons k, player_t player, Bot bot[],Tokens Tok[], int ntokens, int nbots, bool boton, float delta_time,int velocidad);
void generarBots(M_Lab m_Lab, Bot bots[], int nbots, Textures tex, const Vector2i pPlayer);
void generarTokens(M_Lab m_Lab,Tokens tokens[], int ncafe, int ndine, int ncharcos, Textures tex, Vector2i pPlayer);
void generarSalida(Entity *salida, M_Lab m_Lab, const Vector2i pPlayer, Textures tex);
void renderLab(Window window, Entity muros[], int nmuros, Entity salida);


