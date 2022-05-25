#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
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

//Quita la �ltima informaci�n que se ha sacado
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

void renderFondo(Window window, SDL_Texture* tex)
{
    SDL_RenderCopy(window.renderer, tex, NULL, NULL);
}

void renderPause(Window window, SDL_Texture* tex)
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
    if(event.type==SDL_QUIT) //Permite cerrarla ventana
    return 0;
}

int ComprobarMuros(int new_x, int new_y, Vector2f v2, const Entity* muros, int num_muros)//Comprueba que no haya muros delante
{
    SDL_Rect target;
    target.x = new_x;
    target.y = new_y;
    target.w = v2.x;
    target.h = v2.y;

    for (int j = 0; j < num_muros; ++j) {
        SDL_Rect aux;
        if (SDL_IntersectRect(&muros[j].dst, &target, &aux) == SDL_TRUE) {
            return 1;
        }
    }
    return 0;
}

bool UpdateKeys(key_buttons* k, SDL_Event event, bool* game, bool* update)
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
                    k->ESC = true;
                    if (!k->ESC_PREV) {
                        k->ESC_PREV = true;
                        return false;
                    } else {
                        return true;
                    }
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
                    k->ESC = false;
                    k->ESC_PREV = false;
                    return false;
            }
            break;
    }
}

bool boton_invisibilidad (bool boton, float game_time, float *tiempo_boton_in, float *tiempo_boton_fin, float *tiempo_fin_invisibilidad, bool *invisibilidad, Mix_Chunk *invisi)
{

int game_time_int, tiempo_boton_fin_int, tiempo_fin_invis_int;
game_time_int = (int)game_time;
tiempo_boton_fin_int = (int) *tiempo_boton_fin;
tiempo_fin_invis_int = (int) *tiempo_fin_invisibilidad;


    if (boton == true )//Si botón activado
    {
        *tiempo_boton_in = game_time;
        *tiempo_boton_fin = *tiempo_boton_in + 10; //Habrá 10 segundos de cooldown del botón, son 3 tras acabar la invisibilidad
        *tiempo_fin_invisibilidad = *tiempo_boton_in + 7;
        *invisibilidad = invisibility(game_time, tiempo_fin_invis_int, boton, invisi);
        //printf("False por primera vez de nuevo------------\n");
        return false;

    }
    else //Si botón desactivado
    {
        if (game_time_int == tiempo_fin_invis_int)
        {
            *invisibilidad = false;
        }
        if (game_time_int == tiempo_boton_fin_int)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

int max(int v[],int n)
{
    int i, aux;
    aux=v[0];
    for(i=1;i<n;i++)
    {
        if(aux<v[i])
            aux=v[i];
    }
    return aux;
}

int numDigits(long long int x) {
    int digits = 0;
    while (x > 0) {
        x /= 10;
        digits++;
    }

    return digits;
}
