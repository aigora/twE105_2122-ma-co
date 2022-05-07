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
{//A�adir la particularidad de que si hay muros no lo haga
    Vector2f vect;
    switch(num_aleat)
    {
        case 1://Derecha
            bot->texture.x += /*(int)*/ VELOCITY/**delta_time*/;
            break;
        case 2://Izquierda
            bot->texture.x -= /*(int)*/ VELOCITY/**delta_time*/;
            break;
        case 3://Arriba
            bot->texture.y += /*(int)*/ VELOCITY/**delta_time*/;
            break;
        case 4://Abajo
            bot->texture.y -= /*(int)*/ VELOCITY/**delta_time*/;
            break;
    }
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

    vect.x=vect.x/playerDist(v1, v2); //Puede generarse un problema con el rango debido a
    vect.y=vect.y/playerDist(v1, v2); //que la funci�n distancia no hace exactamente lo que necesitariamos aqu�

    //printf("%f  ", vect.x);

    if(vect.x>0)
    new_x-= /*(int)*/ VELOCITY; //Preguntar por el delta_time porque deja de funcionar perseguir
    if(vect.x<0)
    new_x+= /*(int)*/ VELOCITY;
    if(vect.y>0)
    new_y-= /*(int)*/ VELOCITY;
    if(vect.y<0)
    new_y+= /*(int)*/ VELOCITY;

    SDL_Rect target;
    target.x = new_x;
    target.y = new_y;
    target.w = v2->texture.w;
    target.h = v2->texture.h;

    // Si el rectangulo NO intersecciona con los muros, se avanza
    for (int j = 0; j < num_muros; ++j) {
        SDL_Rect aux;
        if (SDL_IntersectRect(&muros[j].dst, &target, &aux) == SDL_TRUE) {
            return;
        }
    }

    v2->texture.x = new_x;
    v2->texture.y = new_y;
}
else
    return;
}

