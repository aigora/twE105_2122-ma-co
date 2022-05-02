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

#define VELOCITY 5

// Devuelve TRUE si el punto `point` está dentro del rectángulo `rectangle`.
static bool is_inside(Vector2i point, SDL_Rect rectangle) {
    if ((point.y >= rectangle.y) && (point.y <= (rectangle.y + rectangle.h))) {
        if ((point.x >= rectangle.x) && (point.x <= (rectangle.x + rectangle.w))) {
            return true;
        }
    }

    return false;
}

//CRea la imagen del jugador con sus dimensiones y su posicion inicial
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

//Permite el movimiento del jugador, teniendo en cuenta los muros
//
void movePlayer(player_t* player, const Entity* muros, int num_muros, player_direction_t direction) {
    bool can_move = true;
    Vector2i target;
    switch (direction) {
        case MOVEMENT_UP:
            target.x = player->texture.x;
            target.y = player->texture.y - VELOCITY;
            break;

        case MOVEMENT_DOWN:
            target.x = player->texture.x;
            target.y = player->texture.y + VELOCITY;
            break;

        case MOVEMENT_RIGHT:
            target.x = player->texture.x + VELOCITY;
            target.y = player->texture.y;
            break;

        case MOVEMENT_LEFT:
            target.x = player->texture.x - VELOCITY;
            target.y = player->texture.y;
            break;
    }
    //Recorremos los muros del laberinto
    for (int i = 0; i < num_muros; ++i) {
        Vector2i bottom_right;
        //Dtermina el contorno del jugador
        bottom_right.x = target.x + player->texture.w;
        bottom_right.y = target.y + player->texture.h;
        //Si el contorno del jugador entra en contacto con un muro, no se puede mover
        if (is_inside(target, muros[i].dst) || is_inside(bottom_right, muros[i].dst)) {
            can_move = false;
            break;
        }
    }

    //Si el jugador no entra en contacto con los muros se puede mover
    if (can_move) {
        player->texture.x = target.x;
        player->texture.y = target.y;
    }
}

float playerDist(player_t* v1, bot_struct* v2)//V1=player v2=IA
{
    float mod;
    mod = sqrt(pow((v2->texture.x - v1->texture.x),2) + pow((v2->texture.y - v1->texture.y), 2));
    return mod;
}
