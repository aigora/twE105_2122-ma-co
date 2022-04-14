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

    SDL_Renderer* rend = SDL_CreateRenderer(mainWin, -1, render_flags); //Genera el render, se utilizan ambas flags para acudir al software gr�fico y para hacer la imagen m�s fluida evitando superposici�n de p�xeles (screen terror), respectivamente

    if(!rend)
    {
        printf("Error creando renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(mainWin);
        SDL_Quit();
        return 0;
    }

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

    SDL_Event event; //Creamos una variable de tipo evento
    int SDL_CaptureMouse(SDL_bool enabled);

 while(stage!=1)
    {
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
                if(event.type==SDL_QUIT) //Permite salir de la ventana si se cierra arriba a la derecha
                    stage=1;

                if(mouse_x < 450 && mouse_y > 420 && mouse_x > 120 && mouse_y < 500)
                {
                    if(event.type==SDL_MOUSEBUTTONUP) //JUGAR
                    {
                        stage=2;
                    }
                }
                if(mouse_x > 550 && mouse_y > 420 && mouse_x < 880 && mouse_y < 500) //Selecciona en que parte de la pantalla puedo clickar
                {
                    if(event.type==SDL_MOUSEBUTTONUP) //SCORES
                    {
                        stage=3;
                    }
                }
                if ( mouse_x > 120 && mouse_x < 450 && mouse_y > 520 && mouse_y < 600)
                {
                    if(event.type==SDL_MOUSEBUTTONUP) //AJUSTES
                    {
                        stage=4;
                    }
                }
                if (mouse_x > 550 && mouse_x < 880 && mouse_y > 520 && mouse_y < 600)
                     if(event.type==SDL_MOUSEBUTTONUP) //SALIR
                    {
                        stage=1;
                    }

            }
    }
    int mouse_x, mouse_y, buttons;
    switch (stage)
    {
        case 2://JUGAR
        buttons = SDL_GetMouseState(&mouse_x, &mouse_y); //Adjunta unas coordenadas al mouse


        SDL_Surface* JUGARsurface = IMG_Load("resources/carga.jpg"); //GENERA UNA NUEVA IMAGEN
        SDL_Texture* JUGARtexture = SDL_CreateTextureFromSurface(rend, JUGARsurface); //A�ADE LA NUEVA IMAGEN
        SDL_FreeSurface(JUGARsurface);


        //Dibuja la imagen
        SDL_RenderCopy(rend, JUGARtexture, NULL, NULL);
        SDL_RenderPresent(rend);


        while(SDL_PollEvent(&event))
            {
                if(event.type==SDL_QUIT) //Permite salir de la ventana si se cierra arriba a la derecha
                    stage=1;
            }
            break;
        case 3://SCORES
        buttons = SDL_GetMouseState(&mouse_x, &mouse_y); //Adjunta unas coordenadas al mouse


        SDL_Surface*SCOREsurface = IMG_Load("resources/Proximamente.jpg");
        SDL_Texture* SCOREtexture = SDL_CreateTextureFromSurface(rend, SCOREsurface);
        SDL_FreeSurface(SCOREsurface);


        //Dibuja la imagen
        SDL_RenderCopy(rend, SCOREtexture, NULL, NULL);
        SDL_RenderPresent(rend);


        while(SDL_PollEvent(&event))
            {
                if(event.type==SDL_QUIT) //Permite salir de la ventana si se cierra arriba a la derecha
                    stage=1;
            }
            break;
        case 4://AJUSTES
        buttons = SDL_GetMouseState(&mouse_x, &mouse_y); //Adjunta unas coordenadas al mouse


        SDL_Surface*AJUSTESsurface = IMG_Load("resources/ajustes.jpg");
        SDL_Texture* AJUSTEStexture = SDL_CreateTextureFromSurface(rend, AJUSTESsurface);
        SDL_FreeSurface(AJUSTESsurface);


        //Dibuja la imagen
        SDL_RenderCopy(rend, AJUSTEStexture, NULL, NULL);
        SDL_RenderPresent(rend);


        while(SDL_PollEvent(&event))
            {
                if(event.type==SDL_QUIT) //Permite salir de la ventana con la cruceta
                    stage=1;

                     if(mouse_x < 290 && mouse_y > 450 && mouse_x > 60 && mouse_y < 540)
                {
                    if(event.type==SDL_MOUSEBUTTONUP) //SONIDO
                    {
                        stage=5;
                    }
                }
                if(mouse_x > 400 && mouse_y > 450 && mouse_x < 630 && mouse_y < 540) //Selecciona en que parte de la pantalla puedo clickar
                {
                    if(event.type==SDL_MOUSEBUTTONUP) //PERSONAJE
                    {
                        stage=6;
                    }
                }
                if ( mouse_x > 710 && mouse_x < 900 && mouse_y > 450 && mouse_y < 540)
                {
                    if(event.type==SDL_MOUSEBUTTONUP) //USUARIO
                    {
                        stage=7;
                    }
                }
                if ( mouse_x > 830 && mouse_x < 980 && mouse_y > 575 && mouse_y < 600)
                {
                    if(event.type==SDL_MOUSEBUTTONUP) //VOLVER
                    {
                        stage=0;
                    }
                }


            }
            break;
        case 5://SONIDO
        buttons = SDL_GetMouseState(&mouse_x, &mouse_y); //Adjunta unas coordenadas al mouse

        SDL_Surface*SONIDOsurface = IMG_Load("resources/ajustessinson.jpg");
        SDL_Texture* SONIDOtexture = SDL_CreateTextureFromSurface(rend, SONIDOsurface);
        SDL_FreeSurface(SONIDOsurface);
        //QUEDA PENDIENTE QUITAR EL SONIDO UNA VEZ SE ACTIVE DESDE INICIO

        //Dibuja la imagen
        SDL_RenderCopy(rend, SONIDOtexture, NULL, NULL);
        SDL_RenderPresent(rend);

        //VER C�MO IMPLEMENTAR ESTA PANTALLA DE MANERA RECURSIVA (BUCLES)
        while(SDL_PollEvent(&event))
            {
                if(event.type==SDL_QUIT) //Permite salir de la ventana con la cruceta
                    stage=1;

                     if(mouse_x < 290 && mouse_y > 450 && mouse_x > 60 && mouse_y < 540)
                {
                    if(event.type==SDL_MOUSEBUTTONUP) //SONIDO
                    {
                        stage=8;
                    }
                }
                if(mouse_x > 400 && mouse_y > 450 && mouse_x < 630 && mouse_y < 540) //Selecciona en que parte de la pantalla puedo clickar
                {
                    if(event.type==SDL_MOUSEBUTTONUP) //PERSONAJE
                    {
                        stage=6;
                    }
                }
                if ( mouse_x > 710 && mouse_x < 900 && mouse_y > 450 && mouse_y < 540)
                {
                    if(event.type==SDL_MOUSEBUTTONUP) //USUARIO
                    {
                        stage=7;
                    }
                }
                 if ( mouse_x > 830 && mouse_x < 980 && mouse_y > 575 && mouse_y < 600)
                {
                    if(event.type==SDL_MOUSEBUTTONUP) //VOLVER
                    {
                        stage=0;
                    }
                }


            }
            break;
            case 6://PERSONAJE
        buttons = SDL_GetMouseState(&mouse_x, &mouse_y); //Adjunta unas coordenadas al mouse

        SDL_Surface*PERSONAJEsurface = IMG_Load("resources/Proximamente.jpg");
        SDL_Texture* PERSONAJEtexture = SDL_CreateTextureFromSurface(rend, PERSONAJEsurface);
        SDL_FreeSurface(PERSONAJEsurface);


        //Dibuja la imagen
        SDL_RenderCopy(rend, PERSONAJEtexture, NULL, NULL);
        SDL_RenderPresent(rend);


        while(SDL_PollEvent(&event))
            {
                if(event.type==SDL_QUIT) //Permite salir de la ventana si se cierra arriba a la derecha
                    stage=1;
            }
            break;
            case 7://USUARIO
        buttons = SDL_GetMouseState(&mouse_x, &mouse_y); //Adjunta unas coordenadas al mouse

        SDL_Surface*USUARIOsurface = IMG_Load("resources/Proximamente.jpg");
        SDL_Texture* USUARIOtexture = SDL_CreateTextureFromSurface(rend, USUARIOsurface);
        SDL_FreeSurface(USUARIOsurface);


        //Dibuja la imagen
        SDL_RenderCopy(rend, USUARIOtexture, NULL, NULL);
        SDL_RenderPresent(rend);


        while(SDL_PollEvent(&event))
            {
                if(event.type==SDL_QUIT) //Permite salir de la ventana si se cierra arriba a la derecha
                    stage=1;
            }
            break;
            case 8:
                buttons = SDL_GetMouseState(&mouse_x, &mouse_y); //Adjunta unas coordenadas al mouse

        SDL_Surface*AJUSTES2surface = IMG_Load("resources/ajustes.jpg");
        SDL_Texture* AJUSTES2texture = SDL_CreateTextureFromSurface(rend, AJUSTES2surface);
        SDL_FreeSurface(AJUSTES2surface);


        //Dibuja la imagen
        SDL_RenderCopy(rend, AJUSTES2texture, NULL, NULL);
        SDL_RenderPresent(rend);


        while(SDL_PollEvent(&event))
            {
                if(event.type==SDL_QUIT) //Permite salir de la ventana si se cierra arriba a la derecha
                    stage=1;
            }
            break;



        while(SDL_PollEvent(&event))
            {
                if(event.type==SDL_QUIT) //Permite salir de la ventana si se cierra arriba a la derecha
                    stage=1;
            }
            break;

    }
    }

    //Elimina todos los objetos de SDL
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(mainWin);
    SDL_Quit();
    return 0;

}
