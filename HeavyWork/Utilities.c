#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> //Preguntar si su uso está permitido
#include <time.h> //Preguntar si su uso está permitido
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_audio.h>
#include "Utilities.h"



void pushStack(Stack stack[],int x, int y)
{
    int last=0;
    while(stack[last].last==false)
        last++;

    stack[last].last=false;
    stack[last+1].x=x;
    stack[last+1].y=y;
    stack[last+1].last=true;

}

//Quita la última información que se ha sacado
int popStack(Stack stack[])
{
    int last=0;
    while(stack[last].last==false)
        last++;

    stack[last].last=false;
    stack[last-1].last=true;
    return last;

}


//Guarda texturas en el renderer
SDL_Texture* loadTexture(const char p_file[], Window window)
{
    SDL_Texture* texture = NULL;
    texture = IMG_LoadTexture(window.renderer,p_file);
    if(texture==NULL)
        printf("Error obteniendo textura: %s",SDL_GetError());
    return texture;
}

void renderLab(Window window, Entity muros[], int nmuros)
{
    int i;
    for(i=0;i<nmuros;i++)
            SDL_RenderCopy(window.renderer, muros[i].tex, &muros[i].src, &muros[i].dst);
}

void renderFondo(Window window, SDL_Texture* tex)
{
    SDL_RenderCopy(window.renderer, tex, NULL, NULL);
}

void imprimirImagen(Window window, SDL_Texture* tex)
{
    SDL_RenderClear(window.renderer);
    SDL_RenderCopy(window.renderer, tex, NULL, NULL);
    SDL_RenderPresent(window.renderer);
}

int Cerrax (SDL_Event event)
{
    if(event.type==SDL_QUIT) //Permite salir de la ventana si se cierra arriba a la derecha
    return 0;
}
int num_al (void) //Para el movimiento aleatorio de los bots
{
    int n;
    srand(time(NULL)); //Genera semilla aleatoria a partir de la hora del equipo
    n = rand() % 4 + 1;//Numero aleatorio entero entre 1 y 4
    return n;
}
int ComprobarMuros(int new_x, int new_y, bot_struct* v2, const Entity* muros, int num_muros)
{
    SDL_Rect target;
    target.x = new_x;
    target.y = new_y;
    target.w = v2->texture.w;
    target.h = v2->texture.h;

    // Si el rectangulo NO intersecciona con los muros, se avanza
    for (int j = 0; j < num_muros; ++j) {
        SDL_Rect aux;
        if (SDL_IntersectRect(&muros[j].dst, &target, &aux) == SDL_TRUE) {
            return 1;
        }
    }
    return;
}

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
                                case SDL_SCANCODE_SPACE:
                                    k->SPACE=true;
                                    break;
                                case SDL_SCANCODE_ESCAPE:
                                    k->ESC=true;
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
                                case SDL_SCANCODE_SPACE:
                                    k->SPACE=false;
                                    break;
                                case SDL_SCANCODE_ESCAPE:
                                    k->ESC=false;
                                    break;
                            }
                            break;
                    }
}

bool boton_invisibilidad (bool boton, float game_time, float *tiempo_boton_in, float *tiempo_boton_fin, float *tiempo_fin_invisibilidad, bool *invisibilidad)
{

int game_time_int, tiempo_boton_fin_int, tiempo_fin_invis_int;
game_time_int = (int)game_time;
tiempo_boton_fin_int = (int) *tiempo_boton_fin;
tiempo_fin_invis_int = (int) *tiempo_fin_invisibilidad;


    if (boton == true )//Si botón activado
    {
        *tiempo_boton_in = game_time;
        *tiempo_boton_fin = *tiempo_boton_in + 15; //Habrá 15 segundos de cooldown del botón, son 10 tras acabar la invisibilidad
        *tiempo_fin_invisibilidad = *tiempo_boton_in + 5;
        *invisibilidad = invisibility(game_time, tiempo_fin_invis_int, boton);
        //printf("False por primera vez de nuevo------------\n");
        return false;

    }
    else //Si boton desactivado
    {
        if (game_time_int == tiempo_fin_invis_int)
        {
            *invisibilidad = false;
        }
        if (game_time_int == tiempo_boton_fin_int)
        {
            //printf("True de nuevo-----------------------------\n");
            return true;
        }
        else
        {
            //printf("False\n");
            return false;

        }
    }
}

