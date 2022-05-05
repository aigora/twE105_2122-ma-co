#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_audio.h>
#include "Enemies.h"
#include "Utilities.h"
#include "Player.h"

#define WIDTH  25
#define HEIGHT 25
#define VELOCITY 2


bot_struct* bot_creator(Vector2i initial_position)
{
    bot_struct* bot = (bot_struct*) malloc(sizeof(bot_struct));


    bot->texture.x = initial_position.x;
    bot->texture.y = initial_position.y;
    bot->texture.w = WIDTH;
    bot->texture.h = HEIGHT;

    return bot;
}

void renderBot(bot_struct* bot, Window window)
{
    SDL_SetRenderDrawColor(window.renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(window.renderer, &bot->texture);
}


void mov_bot (int num_aleat, bot_struct* bot)
{//Añadir la particularidad de que si hay muros no lo haga
    switch (num_aleat)
    {
        case 1://Derecha
            bot->texture.x += VELOCITY;
    break;
        case 2://Izquierda
            bot->texture.x -= VELOCITY;

    break;
        case 3://Arriba
            bot->texture.y += VELOCITY;
    break;
        case 4://Abajo
            bot->texture.y -= VELOCITY;
    break;
    }
}

void perseguir(player_t* v1, bot_struct* v2)
{
    Vector2f vect;
    vect.x=v2->texture.x - v1->texture.x;
    vect.y=v2->texture.y - v1->texture.y;

    vect.x=vect.x/playerDist(v1, v2);
    vect.y=vect.y/playerDist(v1, v2);
    printf("%f  ", vect.x);
    if(vect.x>0)
    v2->texture.x-= VELOCITY/2;
    if(vect.x<0)
    v2->texture.x+= VELOCITY/2;
    if(vect.y>0)
    v2->texture.y-= VELOCITY/2;
    if(vect.y<0)
    v2->texture.y+= VELOCITY/2;


}

