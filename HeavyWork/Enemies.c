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


bot_struct* bot_creator(Vector2i initial_position, SDL_Texture* bot_texture)
{
    int const WIDTH = 40;
    int const HEIGHT = 70;

    bot_struct* bot = (bot_struct*) malloc(sizeof(bot_struct));


    bot->texture.x = initial_position.x;
    bot->texture.y = initial_position.y;
    bot->texture.w = WIDTH;
    bot->texture.h = HEIGHT;

    bot->texture_bot = bot_texture;

    return bot;
}

void renderBot(bot_struct* bot, Window window)
{
    SDL_SetRenderDrawColor(window.renderer, 0, 0, 0, 0);
    SDL_RenderFillRect(window.renderer, &bot->texture);
    SDL_SetRenderDrawBlendMode(window.renderer,  SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(window.renderer, bot->texture_bot, NULL, &bot->texture);

}


void mov_bot (int num_aleat, bot_struct* bot, const Entity* muros, int num_muros,float delta_time)
{//Añadir la particularidad de que si hay muros no lo haga
    int new_x = bot->texture.x;
    int new_y = bot->texture.y;

    const float velocity=150;

    const int position=(int)(velocity*delta_time);

    switch(num_aleat)
    {
        case 1://Derecha
            new_x += position;
            break;
        case 2://Izquierda
            new_x -= position;
            break;
        case 3://Arriba
            new_y += position;
            break;
        case 4://Abajo
            new_y -= position;
            break;
    }
    if(ComprobarMuros(new_x, new_y, bot, muros, num_muros)==1)
        return;

    bot->texture.x = new_x;
    bot->texture.y = new_y;
}

void perseguir(player_t* v1, bot_struct* v2, const Entity* muros, int num_muros,float delta_time, int invisibilidad)
{

    const float velocity=150;
    const int position=(int)(velocity*delta_time);

    if(invisibilidad == 0)
    {
    Vector2f vect;
    int new_x = v2->texture.x;
    int new_y = v2->texture.y;

    vect.x=new_x - (v1->texture.x);
    vect.y=new_y - (v1->texture.y);

    if(vect.x>0)
    {
        new_x-= position;
        if(ComprobarMuros(new_x, new_y, v2, muros, num_muros)==1)
            new_x+=position;
    }
    if(vect.x<0)
    {
        new_x+= position;
        if(ComprobarMuros(new_x, new_y, v2, muros, num_muros)==1)
            new_x-=position;
    }
    if(vect.y>0)
    {
        new_y-= position;
        if(ComprobarMuros(new_x, new_y, v2, muros, num_muros)==1)
            new_y+=position;
    }
    if(vect.y<0)
    {
        new_y+= position;
        if(ComprobarMuros(new_x, new_y, v2, muros, num_muros)==1)
            new_y-=position;
    }

    v2->texture.x = new_x;
    v2->texture.y = new_y;
}
else
    return;
}

