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
#define HEIGHT 70

#define VIDA_WIDTH      30
#define VIDA_HEIGHT     30

#define VIDA_X_INITIAL  25
#define VIDA_Y          25

#define VIDA_MARGIN     10

player_t* newPlayer(Vector2i initial_position, int num_vidas, SDL_Texture* life_texture,SDL_Texture* player_texture,SDL_Texture* playerinv_texture) {
    player_t* player = (player_t*) malloc(sizeof(player_t));

    player->texture.x = initial_position.x;
    player->texture.y = initial_position.y;
    player->texture.w = WIDTH;
    player->texture.h = HEIGHT;

    //Conserva la posicion inicial del personaje
    player->pos_inicial.x = initial_position.x;
    player->pos_inicial.y = initial_position.y;

    player->num_vidas = num_vidas;
    player->texture_life = life_texture;
    player->texture_player = player_texture;
    player->texture_playerinv = playerinv_texture;

    return player;
}

void renderPlayer(player_t* player, Window window, bool invisibilidad)
{
    SDL_SetRenderDrawColor(window.renderer, 0, 0, 0, 0);
    SDL_RenderFillRect(window.renderer, &player->texture);
    SDL_SetRenderDrawBlendMode(window.renderer,  SDL_BLENDMODE_BLEND);

    if (invisibilidad == true)
    {
    SDL_RenderCopy(window.renderer, player->texture_playerinv, NULL, &player->texture);
    }
    else
    {
    SDL_RenderCopy(window.renderer, player->texture_player, NULL, &player->texture);

    }

    // Renderizar el numero de vidas.
    for (int i = 0; i < player->num_vidas; i++) {
        SDL_Rect dst;
        dst.w = VIDA_WIDTH;
        dst.h = VIDA_HEIGHT;
        dst.x = VIDA_X_INITIAL + (i * VIDA_MARGIN) + (i * VIDA_WIDTH);
        dst.y = VIDA_Y;
        SDL_RenderCopy(window.renderer, player->texture_life, NULL, &dst);
    }
}

// Mover el jugador en la direccion indicada.
void movePlayer(player_t* player, const Entity* muros, int num_muros, player_direction_t direction, float delta_time) {

    const float velocity = 150;
    const int position=(int)(velocity*delta_time);
    Vector2f v;
    v.x = player->texture.w;
    v.y = player->texture.h;

    int new_x = player->texture.x;
    int new_y = player->texture.y;

    switch (direction) {
    case MOVEMENT_UP:
        new_y-= position;
        break;
    case MOVEMENT_DOWN:
        new_y+= position;
        break;
    case MOVEMENT_RIGHT:
        new_x+= position;
        break;
    case MOVEMENT_LEFT:
        new_x-= position;
        break;
            }

        //Creaci�n de un nuevo rectangulo desplazado sobre el que se detectan las colisiones
        if(ComprobarMuros(new_x, new_y, v, muros, num_muros)==1)
            return;

        player->texture.x = new_x;
        player->texture.y = new_y;
}

float playerDist(player_t* v1, Bot bot, const Entity* muros, int num_muros)
{
    float mod;
    int i=0;
    mod = sqrt(pow((bot.entity.dst.x+12 - (v1->texture.x+20)),2) + pow((bot.entity.dst.y+12 - (v1->texture.y+20)), 2));

    Vector2f new1, new2;

    new1.x= v1->texture.x+20;
    new1.y= v1->texture.y+20;
    new2.x= bot.entity.dst.x+12;
    new2.y= bot.entity.dst.y+12;

    Vector2f vect, resum;

    vect.x=new2.x - new1.x;
    vect.y=new2.y - new1.y;
    resum.x=new1.x;
    resum.y=new1.y;


    while( i<100)
    {
        resum.x+=vect.x/100;
        resum.y+=vect.y/100;
        i++;


        SDL_Rect target;
        target.x = resum.x;
        target.y = resum.y;
        target.w = 1;
        target.h = 1;

        // Si el rectangulo NO intersecciona con los muros, se avanza
        for (int j = 0; j < num_muros; ++j) {
            SDL_Rect aux;
            if (SDL_IntersectRect(&muros[j].dst, &target, &aux) == SDL_TRUE) {
                return 100000;
            }
        }
    }

    return mod;
}

//Por cada vez que el jugador se choque con un enemigo pierde una vida y vuelve a la posicion inicial (Hasta que se le acaban las vidas)
bool playerKill(player_t* player) {

    player->num_vidas--;

    return (player->num_vidas > 0);
}

bool invisibility(float time, int tiempo_fin_invisibilidad, bool boton)
{
    int t_ent,i;
    t_ent = (int) time;

    if(boton == 1)
    {
        return true;
    }
    else if (time<tiempo_fin_invisibilidad)
        {
            return true;
        }
        else return false;

}
//La funci�n invisibility funciona como booleano, si el tiempo desde el inicio de juego (game_time) es divisible por 60 se activa
//la invisibilidad. Al hacer la conversi�n de float a entero, mientras time se encuentre en [n*60,n*70],
//la invisibilidad estar� activada (10 segundos de invisibilidad).



