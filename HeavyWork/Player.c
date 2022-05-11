#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_audio.h>
#include "Player.h"
#include "Utilities.h"

#define WIDTH  70
#define HEIGHT 70

#define VELOCITY 2

#define VIDA_WIDTH      40
#define VIDA_HEIGHT     40

#define VIDA_X_INITIAL  25
#define VIDA_Y          25

#define VIDA_MARGIN     10

player_t* newPlayer(Vector2i initial_position, int num_vidas, SDL_Texture* life_texture,SDL_Texture* player_texture) {
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

    return player;
}

void renderPlayer(player_t* player, Window window) {
    SDL_SetRenderDrawColor(window.renderer, 0, 0, 0, 0);
    SDL_RenderFillRect(window.renderer, &player->texture);
    SDL_SetRenderDrawBlendMode(window.renderer,  SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(window.renderer, player->texture_player, NULL, &player->texture);


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

    int new_x = player->texture.x;
    int new_y = player->texture.y;
    switch (direction) {
    case MOVEMENT_UP:
        new_y-= (int)VELOCITY*delta_time;
        break;
    case MOVEMENT_DOWN:
        new_y+= (int)VELOCITY*delta_time;
        break;
    case MOVEMENT_RIGHT:
        new_x+= (int)VELOCITY*delta_time;
        break;
    case MOVEMENT_LEFT:
        new_x-= (int)VELOCITY*delta_time;
        break;
            }

        //Creaci�n de un nuevo rectangulo desplazado sobre el que se detectan las colisiones
        if(ComprobarMuros(new_x, new_y, player, muros, num_muros)==1)
            return;

        player->texture.x = new_x;
        player->texture.y = new_y;
}

float playerDist(player_t* v1, bot_struct* v2, const Entity* muros, int num_muros)//V1=player v2=IA
{
    float mod;
    int i=0;
    mod = sqrt(pow((v2->texture.x+12 - (v1->texture.x+20)),2) + pow((v2->texture.y+12 - (v1->texture.y+20)), 2));

    Vector2f new1, new2;

    new1.x= v1->texture.x+20;
    new1.y= v1->texture.y+20;
    new2.x= v2->texture.x+12;
    new2.y= v2->texture.y+12;

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
    player->texture.x = player->pos_inicial.x;
    player->texture.y = player->pos_inicial.y;

    player->num_vidas--;

    return (player->num_vidas > 0);
}

int invisibility(float time, int *aux_invisibilidad, int invisibilidad)
{
    int t_ent,i;
    t_ent = (int) time;

    if(invisibilidad == 0)
    {
        if ((t_ent % 60 == 0)&&(t_ent>0))
    {
        *aux_invisibilidad = t_ent+10; //Como queremos que dure 10s la invisibilidad, aux_invisibilidad es el tiempo de inicio m�s 10
        return 1;//Invisible
    }
    else
        return 0;
    }
    if (invisibilidad == 1)
    {
        if (t_ent == *aux_invisibilidad)
        {
            return 0;
        }
    else return 1;
    }

}
//La funci�n invisibility funciona como booleano, si el tiempo desde el inicio de juego (game_time) es divisible por 60 se activa
//la invisibilidad. Al hacer la conversi�n de float a entero, mientras time se encuentre en [n*60,n*70],
//la invisibilidad estar� activada (10 segundos de invisibilidad).

void UpdateKeys(key_buttons* k, SDL_Event event, bool* game, bool* update)
{
                    switch(event.type)
                    {
                        case SDL_KEYDOWN:

                            switch (event.key.keysym.scancode)
                            {
                                case SDL_SCANCODE_W:
                                case SDL_SCANCODE_UP:
                                    k->W=true;
                                    break;
                                case SDL_SCANCODE_A:
                                case SDL_SCANCODE_LEFT:
                                    k->A=true;
                                    break;
                                case SDL_SCANCODE_S:
                                case SDL_SCANCODE_DOWN:
                                    k->S=true;
                                    break;
                                case SDL_SCANCODE_D:
                                case SDL_SCANCODE_RIGHT:
                                    k->D=true;
                                    break;
                            }
                            break;

                         case SDL_KEYUP:

                            switch (event.key.keysym.scancode)
                            {
                                case SDL_SCANCODE_W:
                                case SDL_SCANCODE_UP:
                                    k->W=false;
                                    break;
                                case SDL_SCANCODE_A:
                                case SDL_SCANCODE_LEFT:
                                    k->A=false;
                                    break;
                                case SDL_SCANCODE_S:
                                case SDL_SCANCODE_DOWN:
                                    k->S=false;
                                    break;
                                case SDL_SCANCODE_D:
                                case SDL_SCANCODE_RIGHT:
                                    k->D=false;
                                    break;
                            }
                            break;
                    }
}

