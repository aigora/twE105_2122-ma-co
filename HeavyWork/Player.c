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

#define VELOCITY 200

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
            new_y--;
            break;
        case MOVEMENT_DOWN:
            new_y++;
            break;
        case MOVEMENT_RIGHT:
            new_x++;
            break;
        case MOVEMENT_LEFT:
            new_x--;
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
    mod = sqrt(pow((v2->texture.x - v1->texture.x),2) + pow((v2->texture.y - v1->texture.y), 2));
    return mod;
}

