#include <stdio.h>
#include <stdbool.h>
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
