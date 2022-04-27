#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> //Preguntar si su uso est� permitido
#include <time.h> //Preguntar si su uso est� permitido
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
int num_botsplanta (int nivel) //Para la generaci�n de tantos mobs por platna, mayor conforme el nivel crece
{
    int n;
    srand(time(NULL)); //Genera semilla aleatoria a partir de la hora del equipo
    n = rand() % (nivel + 3) + 1;//Numero aleatorio entre 1 y nivel m�s 3, se comienza en el nivel 1
    return n;
}
