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

float dist(player_t* v1, bot_struct* v2)//V1=player v2=IA
{
    float mod;
    mod = sqrt(pow((v2->texture.x - v1->texture.x),2) + pow((v2->texture.y - v1->texture.y), 2));
    return mod;

}

