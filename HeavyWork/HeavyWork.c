#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>


int main(int argc, char *argv[])
{

    int stage=0;  //Indica en que fase del flowchart estamos

    //Inicializamos SDL y vemos si da error
    if(SDL_Init(SDL_INIT_EVERYTHING)!=0)
    {
        printf("Error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* mainWin = SDL_CreateWindow("HeavyWork", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480 ,0);  //Creamos una ventana

    //Comprueba que se crea correctamente la ventana
    if(!mainWin)
    {
        printf("Error creating window: %s\n", SDL_GetError());
        SDL_Quit();
        return 0;
    }

    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC ; //Genera las opciones para el renderizador

    SDL_Renderer* rend = SDL_CreateRenderer(mainWin, -1, render_flags); //Genera el render

    if(!rend)
    {
        printf("Error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(mainWin);
        SDL_Quit();
        return 0;
    }

    SDL_Surface* surface = IMG_Load("resources/prueba.jpg"); //Carga una superficie con la imagen de prueba en la memoria

    if(!surface)
    {
        printf("Error creating surface: %s\n", SDL_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(mainWin);
        SDL_Quit();
        return 0;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, surface);
    SDL_FreeSurface(surface);
    if(!texture)
    {
        printf("Error creating surface: %s\n", SDL_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(mainWin);
        SDL_Quit();
        return 0;
    }

    SDL_Event event; //Creamos una variable de tipo evento

    //Bucle del juego
    while(stage==0)
    {

        SDL_RenderClear(rend); //Limpia la ventana

        //Dibuja la imagen
        SDL_RenderCopy(rend, texture, NULL, NULL);
        SDL_RenderPresent(rend);

      while(SDL_PollEvent(&event))
        {
            if(event.type==SDL_QUIT)
                stage=1;
        }
    }

    //Elimina todos los objetos de SDL
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(mainWin);
    SDL_Quit();
    return 0;
}
