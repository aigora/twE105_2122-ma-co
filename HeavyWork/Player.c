#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
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

player_t* newPlayer(Vector2i initial_position, int num_vidas, player_textures_t textures, Window win, Vector2i *desfase, int w, int h)
{
    const int wm=235,ww=5;
    float aux;
    player_t* player = (player_t*) malloc(sizeof(player_t));

    aux= win.w/(2.0*wm);

    //Casillas de la izquierda
    if(initial_position.x+0.5<aux)
    {
        player->texture.x = initial_position.x*wm+wm/2-WIDTH/2;
        desfase->x=0;
    //Casillas de la derecha
    }else if(w-(initial_position.x+1-0.5)<aux)
    {
        player->texture.x = win.w-(w-(initial_position.x+1))*wm-wm/2-ww+WIDTH/2;
        desfase->x=(initial_position.x+1)*(wm-ww)+ww-win.w;
    //El resto de casillas
    }else
    {
        player->texture.x = win.w/2-WIDTH/2;
        desfase->x=initial_position.x*(wm-ww)+wm/2-win.w/2;
    }

    aux=win.h/(2.0*wm);

    //Casillas de arriba
    if(initial_position.y+0.5<aux)
    {
        player->texture.y = initial_position.y*wm+wm/2-HEIGHT/2;
        desfase->y=0;
    //Casillas de abajo
    }else if(h-(initial_position.y+1-0.5)<aux)
    {
        player->texture.y = win.h-(h-(initial_position.y+1))*wm-wm/2-ww+HEIGHT/2;
        desfase->y=(initial_position.y+1)*(wm-ww)+ww-win.h;
    //El resto de casillas
    }else
    {
        player->texture.y = win.h/2-HEIGHT/2;
        desfase->y=initial_position.y*(wm-ww)+wm/2-win.h/2;
    }

    player->texture.w = WIDTH;
    player->texture.h = HEIGHT;

    player->num_vidas = num_vidas;
    player->direction = MOVEMENT_DOWN;

    player->textures.life = textures.life;
    player->textures.player_right = textures.player_right;
    player->textures.player_left = textures.player_left;
    player->textures.player_up = textures.player_up;
    player->textures.player_down = textures.player_down;

    player->textures.player_inv_right = textures.player_inv_right;
    player->textures.player_inv_left = textures.player_inv_left;
    player->textures.player_inv_up = textures.player_inv_up;
    player->textures.player_inv_down = textures.player_inv_down;


    return player;
}

void renderPlayer(player_t* player, Window window, bool invisibilidad)
{
    SDL_SetRenderDrawColor(window.renderer, 0, 0, 0, 0);
    SDL_RenderFillRect(window.renderer, &player->texture);
    SDL_SetRenderDrawBlendMode(window.renderer,  SDL_BLENDMODE_BLEND);

    SDL_Texture* texture_player = NULL;
    if (invisibilidad == true)
    {
        switch (player->direction) {
            case MOVEMENT_UP:
                texture_player = player->textures.player_inv_up;
                break;
            case MOVEMENT_DOWN:
                texture_player = player->textures.player_inv_down;
                break;
            case MOVEMENT_RIGHT:
                texture_player = player->textures.player_inv_right;
                break;
            case MOVEMENT_LEFT:
                texture_player = player->textures.player_inv_left;
                break;}
    }
    else
    {
        switch (player->direction) {
            case MOVEMENT_UP:
                texture_player = player->textures.player_up;
                break;
            case MOVEMENT_DOWN:
                texture_player = player->textures.player_down;
                break;
            case MOVEMENT_RIGHT:
                texture_player = player->textures.player_right;
                break;
            case MOVEMENT_LEFT:
                texture_player = player->textures.player_left;
                break;
        }
    }

    SDL_RenderCopy(window.renderer, texture_player, NULL, &player->texture);

    // Renderizar el numero de vidas.
    for (int i = 0; i < player->num_vidas; i++) {
        SDL_Rect dst;
        dst.w = VIDA_WIDTH;
        dst.h = VIDA_HEIGHT;
        dst.x = VIDA_X_INITIAL + (i * VIDA_MARGIN) + (i * VIDA_WIDTH);
        dst.y = VIDA_Y;
        SDL_RenderCopy(window.renderer, player->textures.life, NULL, &dst);
    }
}

float playerDist(player_t* v1, Bot bot, const Entity* muros, int num_muros)
{
    float mod;
    int i=0;
    mod = sqrt(pow((bot.entity.dst.x+12 - (v1->texture.x+20)),2) + pow((bot.entity.dst.y+12 - (v1->texture.y+20)), 2));

    Vector2f new1, new2;

    new1.x= v1->texture.x+20;//Generamos un punto central en cada entidad
    new1.y= v1->texture.y+20;
    new2.x= bot.entity.dst.x+12;
    new2.y= bot.entity.dst.y+12;

    Vector2f vect;

    vect.x=new2.x - new1.x;
    vect.y=new2.y - new1.y;

    for(i=0; i<100; i++)
    {
        new1.x+=vect.x/100;
        new1.y+=vect.y/100;

        SDL_Rect target;
        target.x = new1.x;
        target.y = new1.y;
        target.w = 1;
        target.h = 1;

        // Si el rectangulo NO intersecciona con los muros, se avanza
        for (int j = 0; j < num_muros; ++j)
        {
            SDL_Rect aux;
            if (SDL_IntersectRect(&muros[j].dst, &target, &aux) == SDL_TRUE)
                return 100000;
        }
    }
    return mod;
}

//El jugador pierde una vida cuando es tocado por un enemigo
bool playerKill(player_t* player)
{
    player->num_vidas--;
    return (player->num_vidas > 0);
}

bool invisibility(float time, int tiempo_fin_invisibilidad, bool boton, Mix_Chunk *invisi)
{
    int t_ent;
    t_ent = (int) time;

    if(boton == 1)
    {
        Mix_PlayChannel( -1, invisi, 0 );
        return true;
    }
    else if (t_ent<tiempo_fin_invisibilidad)
        {
            return true;
        }
        else return false;
}

void playerSetDirection(player_t* player, player_direction_t direction) {
    player->direction = direction;
}

int ColisionPlayer(const player_t player, Entity entity)
{
    SDL_Rect aux;
    if (SDL_IntersectRect(&player.texture, &entity.dst, &aux) == SDL_TRUE) {
            return 1;}
    else
        return 0;
}
