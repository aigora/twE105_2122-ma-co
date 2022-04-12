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
        printf("Error al iniciar SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* mainWin = SDL_CreateWindow("HeavyWork", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1000, 650, 0); //Despliegue de ventana HeavyWork, centrada en "x" e "y", de dimensiones 900x650 y de 0 flags
    //Comprueba que se crea correctamente la ventana
    if(!mainWin)
    {
        printf("Error al crear la ventana: %s\n", SDL_GetError());
        SDL_Quit();
        return 0;
    }

    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC ; //Genera las opciones para el renderizador

    SDL_Renderer* rend = SDL_CreateRenderer(mainWin, -1, render_flags); //Genera el render, se utilizan ambas flags para acudir al software gráfico y para hacer la imagen más fluida evitando superposición de píxeles (screen terror), respectivamente

    if(!rend)
    {
        printf("Error creando renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(mainWin);
        SDL_Quit();
        return 0;
    }

    SDL_Surface* surface = IMG_Load("resources/Menuinicio.jpg"); //Carga una superficie con la imagen de prueba en la memoria

    if(!surface)
    {
        printf("Error creando surface: %s\n", SDL_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(mainWin);
        SDL_Quit();
        return 0;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, surface);
    SDL_FreeSurface(surface);
    if(!texture)
    {
        printf("Error creando texture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(mainWin);
        SDL_Quit();
        return 0;
    }

    SDL_Event event; //Creamos una variable de tipo evento
    int SDL_CaptureMouse(SDL_bool enabled);


    //Bucle del juego
    while(stage==0)
    {
        int mouse_x, mouse_y;
        int buttons = SDL_GetMouseState(&mouse_x, &mouse_y); //Adjunta unas coordenadas al mouse


        SDL_RenderClear(rend); //Limpia la ventana

        //Dibuja la imagen
        SDL_RenderCopy(rend, texture, NULL, NULL);
        SDL_RenderPresent(rend);




        while(SDL_PollEvent(&event))
        {
            if(event.type==SDL_QUIT)
                stage=1;




            if(mouse_x > 550 && mouse_y > 420 && mouse_x < 880 && mouse_y < 500) //Selecciona en que parte de la pantalla puedo clickar
            {
                if(event.type==SDL_MOUSEBUTTONUP)
                {
                    stage=1;
                }
            }
            if(mouse_x < 450 && mouse_y > 420 && mouse_x > 120 && mouse_y < 500)
            {
                if(event.type==SDL_MOUSEBUTTONUP)
                {
                    stage=1;
                }
            }
        }



    }

    //Elimina todos los objetos de SDL
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(mainWin);
    SDL_Quit();
    return 0;
}
