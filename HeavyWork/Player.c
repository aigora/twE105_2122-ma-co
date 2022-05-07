#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_audio.h>
#include "Player.h"
#include "Utilities.h"

#define WIDTH  40
#define HEIGHT 40

#define VELOCITY 10

player_t* newPlayer(Vector2i initial_position) {
    player_t* player = (player_t*) malloc(sizeof(player_t));

    player->texture.x = initial_position.x;
    player->texture.y = initial_position.y;
    player->texture.w = WIDTH;
    player->texture.h = HEIGHT;

    return player;
}

void renderPlayer(player_t* player, Window window) {
    SDL_SetRenderDrawColor(window.renderer, 0, 0, 180, 255);
    SDL_RenderFillRect(window.renderer, &player->texture);
}

/// Mover el jugador en la direccion indicada.
void movePlayer(player_t* player, const Entity* muros, int num_muros, player_direction_t direction, float delta_time) {
   //el bucle recorre desde 0 hasta la velocidad y se avanza o no para cada interacción del bucle
    for (int i = 0; i < (int)VELOCITY*delta_time; ++i) {
        int new_x = player->texture.x;
        int new_y = player->texture.y;
        switch (direction) {
        case MOVEMENT_UP:
            new_y-= VELOCITY;
            break;
        case MOVEMENT_DOWN:
            new_y+= VELOCITY;
            break;
        case MOVEMENT_RIGHT:
            new_x+= VELOCITY;
            break;
        case MOVEMENT_LEFT:
            new_x-= VELOCITY;
            break;}

            //Creación de un nuevo rectangulo desplazado sobre el que se detectan las colisiones
        SDL_Rect target;
        target.x = new_x;
        target.y = new_y;
        target.w = player->texture.w;
        target.h = player->texture.h;

        // Si el rectangulo NO intersecciona con los muros, se avanza
        for (int j = 0; j < num_muros; ++j) {
            SDL_Rect aux;
            if (SDL_IntersectRect(&muros[j].dst, &target, &aux) == SDL_TRUE) {
                return;
            }
        }

        player->texture.x = new_x;
        player->texture.y = new_y;
    }
}

float playerDist(player_t* v1, bot_struct* v2)//V1=player v2=IA
{
    float mod;
    mod = sqrt(pow((v2->texture.x+12 - (v1->texture.x+20)),2) + pow((v2->texture.y+12 - (v1->texture.y+20)), 2));
    return mod;
}

int invisibility(float time)
{
    int t_ent,i;
    t_ent = (int) time;
       if (((t_ent % 60 == 0)&&(t_ent>0))||((t_ent % 61 == 0)&&(t_ent>0))||((t_ent % 62 == 0)&&(t_ent>0))||((t_ent % 63 == 0)&&(t_ent>0))||((t_ent % 64 == 0)&&(t_ent>0))||((t_ent % 65 == 0)&&(t_ent>0))||((t_ent % 66 == 0)&&(t_ent>0))||((t_ent % 67 == 0)&&(t_ent>0))||((t_ent % 68 == 0)&&(t_ent>0))||((t_ent % 69 == 0)&&(t_ent>0)))
    {
        return 1;
    }
    else
        {
            return 0;
        }
        //La condición del if es completamente momentánea, se ha intentado solucionar con bucle for pero no lo he conseguido. A esperas de
        //ser cambiada al ocurrírseme una solución
}
//La función invisibility funciona como booleano, si el tiempo desde el inicio de juego (game_time) es divisible por 60 se activa
//la invisibilidad. Al hacer la conversión de float a entero, mientras time se encuentre en [n*60,n*70], será divisible y, por
//tanto, la invisibilidad estará activada (10 segundos de invisibilidad).

