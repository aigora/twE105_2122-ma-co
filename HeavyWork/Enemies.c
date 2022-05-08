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
#define VELOCITY 1


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


void mov_bot (int num_aleat, player_t* player, bot_struct* bot, const Entity* muros, int num_muros,float delta_time)
{//Añadir la particularidad de que si hay muros no lo haga
    Vector2f vect;
    int new_x = bot->texture.x;
    int new_y = bot->texture.y;

    switch(num_aleat)
    {
        case 1://Derecha
            new_x += /*(int)*/ VELOCITY/**delta_time*/;
            break;
        case 2://Izquierda
            new_x -= /*(int)*/ VELOCITY/**delta_time*/;
            break;
        case 3://Arriba
            new_y += /*(int)*/ VELOCITY/**delta_time*/;
            break;
        case 4://Abajo
            new_y -= /*(int)*/ VELOCITY/**delta_time*/;
            break;
    }
    if(ComprobarMuros(new_x, new_y, bot, muros, num_muros)==1)
        return;

    bot->texture.x = new_x;
    bot->texture.y = new_y;
}

void perseguir(player_t* v1, bot_struct* v2, const Entity* muros, int num_muros,float delta_time, int invisibilidad)
{
    if(invisibilidad == 0)
    {
    Vector2f vect;
    int new_x = v2->texture.x;
    int new_y = v2->texture.y;

    vect.x=new_x - (v1->texture.x);
    vect.y=new_y - (v1->texture.y);

    //vect.x=vect.x/playerDist(v1, v2, muros, num_muros); //Puede generarse un problema con el rango debido a
    //vect.y=vect.y/playerDist(v1, v2, muros, num_muros); //que la función distancia no hace exactamente lo que necesitariamos aquí

    if(vect.x>0)
    {
        new_x-= /*(int)*/ VELOCITY;//Preguntar por el delta_time porque deja de funcionar perseguir
        if(ComprobarMuros(new_x, new_y, v2, muros, num_muros)==1)
            new_x+=VELOCITY;
    }
    if(vect.x<0)
    {
        new_x+= /*(int)*/ VELOCITY;//Preguntar por el delta_time porque deja de funcionar perseguir
        if(ComprobarMuros(new_x, new_y, v2, muros, num_muros)==1)
            new_x-=VELOCITY;
    }
    if(vect.y>0)
    {
        new_y-= /*(int)*/ VELOCITY;//Preguntar por el delta_time porque deja de funcionar perseguir
        if(ComprobarMuros(new_x, new_y, v2, muros, num_muros)==1)
            new_y+=VELOCITY;
    }
    if(vect.y<0)
    {
        new_y+= /*(int)*/ VELOCITY;//Preguntar por el delta_time porque deja de funcionar perseguir
        if(ComprobarMuros(new_x, new_y, v2, muros, num_muros)==1)
            new_y-=VELOCITY;
    }

    v2->texture.x = new_x;
    v2->texture.y = new_y;
}
else
    return;
}

