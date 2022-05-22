#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_audio.h>
#include "Enemies.h"
#include "Utilities.h"


void bot_creator(Bot bot[], Vector2i p_initial[], SDL_Texture* tex, int nbots)
{
    int i;
    const int w=40,wh=40;
    const int h=70,hh=70;
    for(i=0;i<nbots;i++)
    {
        bot[i].entity.tex=tex;
        SDL_QueryTexture(tex,NULL,NULL,&bot[i].entity.src.w,&bot[i].entity.src.h);
        bot[i].entity.src.x=0;
        bot[i].entity.src.y=0;
        bot[i].entity.dst.x=p_initial[i].x;
        bot[i].entity.dst.y=p_initial[i].y;
        bot[i].entity.dst.w=w;
        bot[i].entity.dst.h=h;
    }

}

void renderBot(Bot bots[], Window window, int nbots)
{
    int i;
    for(i=0;i<nbots;i++)
            SDL_RenderCopy(window.renderer, bots[i].entity.tex, &bots[i].entity.src, &bots[i].entity.dst);
}


void mov_bot (int num_aleat, Bot* bot, const Entity* muros, int num_muros, float delta_time)
{//Añadir la particularidad de que si hay muros no lo haga
    int new_x;
    int new_y;
    Vector2f v;

    const float velocity=150;

    const int position=(int)(velocity*delta_time);

    new_x=bot->entity.dst.x;
    new_y=bot->entity.dst.y;
    v.x=bot->entity.dst.w;
    v.y=bot->entity.dst.h;

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
    if(ComprobarMuros(new_x, new_y, v, muros, num_muros)==1)
        return;

    bot->entity.dst.x=new_x;
    bot->entity.dst.y=new_y;
}

void perseguir(player_t* v1, Bot* bot, const Entity* muros, int num_muros,float delta_time, int invisibilidad)
{
    Vector2f v;
    v.x = bot->entity.dst.w;
    v.y = bot->entity.dst.h;
    const float velocity=150;
    const int position=(int)(velocity*delta_time);

    if(invisibilidad == 0)
    {
    Vector2f vect;
    int new_x = bot->entity.dst.x;
    int new_y = bot->entity.dst.y;

    vect.x=new_x - (v1->texture.x);
    vect.y=new_y - (v1->texture.y);

    if(vect.x>0)
    {
        new_x-= position;
        if(ComprobarMuros(new_x, new_y, v, muros, num_muros)==1)
            new_x+=position;
    }
    if(vect.x<0)
    {
        new_x+= position;
        if(ComprobarMuros(new_x, new_y, v, muros, num_muros)==1)
            new_x-=position;
    }
    if(vect.y>0)
    {
        new_y-= position;
        if(ComprobarMuros(new_x, new_y, v, muros, num_muros)==1)
            new_y+=position;
    }
    if(vect.y<0)
    {
        new_y+= position;
        if(ComprobarMuros(new_x, new_y, v, muros, num_muros)==1)
            new_y-=position;
    }

    bot->entity.dst.x = new_x;
    bot->entity.dst.y = new_y;
}
}

