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

    SDL_Window* mainWin = SDL_CreateWindow("HeavyWork", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1000, 650, 0); //Despliegue de ventana HeavyWork, centrada en "x" e "y", de dimensiones 1000x650 y de 0 flags
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



    SDL_Event event; //Creamos una variable de tipo evento
    int SDL_CaptureMouse(SDL_bool enabled);

 while(stage!=1)
    {
        //Bucle del juego
        while(stage==0)
        {
            SDL_Surface* surface = IMG_Load("resources/Menuinicio2.jpg"); //Carga una superficie con la imagen de prueba en la memoria

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
            int mouse_x, mouse_y;
            while(stage==0){
                int buttons = SDL_GetMouseState(&mouse_x, &mouse_y); //Adjunta unas coordenadas al mouse


                SDL_RenderClear(rend); //Limpia la ventana

                //Dibuja la imagen
                SDL_RenderCopy(rend, texture, NULL, NULL);
                SDL_RenderPresent(rend);


                while(SDL_PollEvent(&event))
                {
                    if(event.type==SDL_QUIT) //Permite salir de la ventana si se cierra arriba a la derecha
                        {
                            stage=1;
                            SDL_DestroyTexture(texture);
                        }

                    if(mouse_x < 450 && mouse_y > 420 && mouse_x > 120 && mouse_y < 500)
                    {
                        if(event.type==SDL_MOUSEBUTTONUP) //JUGAR
                        {
                            stage=2;
                            SDL_DestroyTexture(texture);
                        }
                    }
                    if(mouse_x > 550 && mouse_y > 420 && mouse_x < 880 && mouse_y < 500) //Selecciona en que parte de la pantalla puedo clickar
                    {
                        if(event.type==SDL_MOUSEBUTTONUP) //SCORES
                        {
                            stage=3;
                            SDL_DestroyTexture(texture);
                        }
                    }
                    if ( mouse_x > 120 && mouse_x < 450 && mouse_y > 520 && mouse_y < 600)
                    {
                        if(event.type==SDL_MOUSEBUTTONUP) //AJUSTES
                        {
                            stage=4;
                            SDL_DestroyTexture(texture);
                        }
                    }
                    if (mouse_x > 550 && mouse_x < 880 && mouse_y > 520 && mouse_y < 600)
                    {
                         if(event.type==SDL_MOUSEBUTTONUP) //SALIR
                        {
                            stage=1;
                            SDL_DestroyTexture(texture);
                        }
                    }
                }
            }
    }
    int mouse_x, mouse_y, buttons;
    switch (stage)
    {
        case 2://JUGAR
                while(stage==2){
                    SDL_Surface* surface = IMG_Load("resources/carga.jpg"); //GENERA UNA NUEVA IMAGEN
                    SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, surface); //AÑADE LA NUEVA IMAGEN
                    SDL_FreeSurface(surface);


                    while(stage==2){
                        buttons = SDL_GetMouseState(&mouse_x, &mouse_y); //Adjunta unas coordenadas al mouse

                        //Dibuja la imagen
                        SDL_RenderCopy(rend, texture, NULL, NULL);
                        SDL_RenderPresent(rend);


                        while(SDL_PollEvent(&event))
                            {
                                if(event.type==SDL_QUIT) //Permite salir de la ventana si se cierra arriba a la derecha
                                {
                                    stage=1;
                                    SDL_DestroyTexture(texture);
                                }
                            }
                    }
                }
            break;
        case 3://SCORES
                while(stage==3){
                    SDL_Surface*surface = IMG_Load("resources/Proximamente.jpg");
                    SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, surface); //AÑADE LA NUEVA IMAGEN
                    SDL_FreeSurface(surface);

                     while(stage==3){
                        buttons = SDL_GetMouseState(&mouse_x, &mouse_y); //Adjunta unas coordenadas al mouse

                        //Dibuja la imagen
                        SDL_RenderCopy(rend, texture, NULL, NULL);
                        SDL_RenderPresent(rend);


                        while(SDL_PollEvent(&event))
                            {
                                if(event.type==SDL_QUIT) //Permite salir de la ventana si se cierra arriba a la derecha
                                {
                                    stage=1;
                                    SDL_DestroyTexture(texture);
                                }
                            }
                    }
                }
            break;
        case 4://AJUSTES
                while(stage==4){
                    SDL_Surface*surface = IMG_Load("resources/ajustes.jpg");
                    SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, surface); //AÑADE LA NUEVA IMAGEN
                    SDL_FreeSurface(surface);

                    while(stage==4){
                        buttons = SDL_GetMouseState(&mouse_x, &mouse_y); //Adjunta unas coordenadas al mouse

                        //Dibuja la imagen
                        SDL_RenderCopy(rend, texture, NULL, NULL);
                        SDL_RenderPresent(rend);


                        while(SDL_PollEvent(&event))
                            {
                                if(event.type==SDL_QUIT) //Permite salir de la ventana con la cruceta
                                    {
                                        stage=1;
                                        SDL_DestroyTexture(texture);
                                    }

                                     if(mouse_x < 290 && mouse_y > 450 && mouse_x > 60 && mouse_y < 540)
                                {
                                    if(event.type==SDL_MOUSEBUTTONUP) //SONIDO
                                    {
                                        stage=5;
                                        SDL_DestroyTexture(texture);
                                    }
                                }
                                if(mouse_x > 400 && mouse_y > 450 && mouse_x < 630 && mouse_y < 540) //Selecciona en que parte de la pantalla puedo clickar
                                {
                                    if(event.type==SDL_MOUSEBUTTONUP) //PERSONAJE
                                    {
                                        stage=6;
                                        SDL_DestroyTexture(texture);
                                    }
                                }
                                if ( mouse_x > 710 && mouse_x < 900 && mouse_y > 450 && mouse_y < 540)
                                {
                                    if(event.type==SDL_MOUSEBUTTONUP) //USUARIO
                                    {
                                        stage=7;
                                        SDL_DestroyTexture(texture);
                                    }
                                }
                                if ( mouse_x > 830 && mouse_x < 980 && mouse_y > 575 && mouse_y < 600)
                                {
                                    if(event.type==SDL_MOUSEBUTTONUP) //VOLVER
                                    {
                                        stage=0;
                                        SDL_DestroyTexture(texture);
                                    }
                                }
                            }
                        }
                }
            break;
        case 5://SONIDO
                while(stage==5){
                    SDL_Surface*surface = IMG_Load("resources/ajustessinson.jpg");
                    SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, surface); //AÑADE LA NUEVA IMAGEN
                    SDL_FreeSurface(surface);
                    //QUEDA PENDIENTE QUITAR EL SONIDO UNA VEZ SE ACTIVE DESDE INICIO

                    while(stage==5){
                        buttons = SDL_GetMouseState(&mouse_x, &mouse_y); //Adjunta unas coordenadas al mouse

                        //Dibuja la imagen
                        SDL_RenderCopy(rend, texture, NULL, NULL);
                        SDL_RenderPresent(rend);


                        while(SDL_PollEvent(&event))
                            {
                                if(event.type==SDL_QUIT) //Permite salir de la ventana con la cruceta
                                    {
                                        stage=1;
                                        SDL_DestroyTexture(texture);
                                    }

                                     if(mouse_x < 290 && mouse_y > 450 && mouse_x > 60 && mouse_y < 540)
                                {
                                    if(event.type==SDL_MOUSEBUTTONUP) //SONIDO
                                    {
                                        stage=4;
                                        SDL_DestroyTexture(texture);
                                    }
                                }
                                if(mouse_x > 400 && mouse_y > 450 && mouse_x < 630 && mouse_y < 540) //Selecciona en que parte de la pantalla puedo clickar
                                {
                                    if(event.type==SDL_MOUSEBUTTONUP) //PERSONAJE
                                    {
                                        stage=6;
                                        SDL_DestroyTexture(texture);
                                    }
                                }
                                if ( mouse_x > 710 && mouse_x < 900 && mouse_y > 450 && mouse_y < 540)
                                {
                                    if(event.type==SDL_MOUSEBUTTONUP) //USUARIO
                                    {
                                        stage=7;
                                        SDL_DestroyTexture(texture);
                                    }
                                }
                                 if ( mouse_x > 830 && mouse_x < 980 && mouse_y > 575 && mouse_y < 600)
                                {
                                    if(event.type==SDL_MOUSEBUTTONUP) //VOLVER
                                    {
                                        stage=0;
                                        SDL_DestroyTexture(texture);
                                    }
                                }
                            }
                    }
                }

            break;
            case 6://PERSONAJE
                    while(stage==6){
                        SDL_Surface*surface = IMG_Load("resources/Proximamente.jpg");
                        SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, surface); //AÑADE LA NUEVA IMAGEN
                        SDL_FreeSurface(surface);

                        while(stage==6){
                            buttons = SDL_GetMouseState(&mouse_x, &mouse_y); //Adjunta unas coordenadas al mouse

                        //Dibuja la imagen
                        SDL_RenderCopy(rend, texture, NULL, NULL);
                        SDL_RenderPresent(rend);


                            while(SDL_PollEvent(&event))
                                {
                                    if(event.type==SDL_QUIT) //Permite salir de la ventana si se cierra arriba a la derecha
                                    {
                                        stage=1;
                                        SDL_DestroyTexture(texture);
                                    }
                                }
                        }
                }

            break;
            case 7://USUARIO
                    while(stage==7){
                        SDL_Surface*surface = IMG_Load("resources/Proximamente.jpg");
                        SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, surface); //AÑADE LA NUEVA IMAGEN
                        SDL_FreeSurface(surface);

                        while(stage==7){
                            buttons = SDL_GetMouseState(&mouse_x, &mouse_y); //Adjunta unas coordenadas al mouse

                        //Dibuja la imagen
                        SDL_RenderCopy(rend, texture, NULL, NULL);
                        SDL_RenderPresent(rend);


                            while(SDL_PollEvent(&event))
                                {
                                    if(event.type==SDL_QUIT) //Permite salir de la ventana si se cierra arriba a la derecha
                                    {
                                        stage=1;
                                        SDL_DestroyTexture(texture);
                                    }
                                }
                    }
                }
            break;


    }
    }

    //Elimina todos los objetos de SDL
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(mainWin);
    SDL_Quit();
    return 0;

}
