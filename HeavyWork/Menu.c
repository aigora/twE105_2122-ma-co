#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_audio.h>
#include "Menu.h"
#include "Utilities.h"
#define SPEED 300//velocidad en pixeles por segundo

int menu(Window window, Textures tex, int *personaje, bool sonido, Mix_Music *musica)
{
    int stage=1;
    SDL_Event event;
    int mouse_x, mouse_y;
    int buttons;
    int cierre;

    Mix_PlayMusic( musica, -1 );//Reproduce la música inicialmente
    //Mix_PlayChannel( -1, recoger, 0 );
    int SDL_CaptureMouse(SDL_bool enabled);

    while(true)
    {
        switch (stage)
        {
            case 1:
                while(stage==1)
                {
                    imprimirImagen(window, tex.menu);
                    buttons = SDL_GetMouseState(&mouse_x, &mouse_y);//Adjunta unas coordenadas al mouse

                    while(SDL_PollEvent(&event))
                    {
                        if(event.type==SDL_QUIT)
                            return 0;

                        if(event.type==SDL_MOUSEBUTTONUP)
                        {
                            if(mouse_x < 450 && mouse_y > 420 && mouse_x > 120 && mouse_y < 500) //JUGAR
                            {
                                imprimirImagen(window, tex.carga);
                                SDL_Delay(500);
                                return 2;
                            }
                            if(mouse_x > 550 && mouse_y > 420 && mouse_x < 880 && mouse_y < 500) //SCORES
                                stage = 3;

                            if(mouse_x > 120 && mouse_x < 450 && mouse_y > 520 && mouse_y < 600) //AJUSTES
                                stage = 4;

                            if (mouse_x > 550 && mouse_x < 880 && mouse_y > 520 && mouse_y < 600) //Salir
                            {
                                return 0;
                            }
                        }
                    }
                }
                break;

            case 3://SCORES
                while(stage==3)
                {
                    imprimirImagen(window, tex.prox);
                    buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

                    while(SDL_PollEvent(&event))
                    {
                        if(event.type==SDL_QUIT)
                            return 0;
                    }
                }
                break;

            case 4://AJUSTES

                imprimirImagen(window,tex.ajustes);
                while(stage==4)
                {
                    buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

                    if (sonido == true)
                        imprimirImagen(window, tex.ajustes);

                    if (sonido == false)
                        imprimirImagen(window, tex.ajusnos);

                    while(SDL_PollEvent(&event))
                    {
                        if(event.type==SDL_QUIT)
                            return 0;

                        if(event.type==SDL_MOUSEBUTTONUP)
                        {
                            if(mouse_x < 290 && mouse_y > 450 && mouse_x > 60 && mouse_y < 540)//SONIDO
                            {
                                if (sonido == false)
                                {
                                    sonido=true;
                                    Mix_ResumeMusic();
                                }
                                else
                                {
                                    sonido=false;
                                    Mix_PauseMusic();
                                }
                            }
                            if(mouse_x > 400 && mouse_y > 450 && mouse_x < 630 && mouse_y < 540) //Selecciona en que parte de la pantalla puedo clickar
                                stage=6;

                            if ( mouse_x > 710 && mouse_x < 900 && mouse_y > 450 && mouse_y < 540)
                                stage=7;

                            if ( mouse_x > 830 && mouse_x < 980 && mouse_y > 575 && mouse_y < 600)
                                stage=1;

                        }
                    }
                }
                break;

            case 6://AYUDA
                while(stage==6)
                {
                        imprimirImagen(window, tex.ayuda);

                    buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

                    while(SDL_PollEvent(&event))
                    {
                        if(event.type==SDL_QUIT)
                            return 0;

                        if(event.type==SDL_MOUSEBUTTONUP)
                        {
                            if ( mouse_x > 770 && mouse_x < 930 && mouse_y > 575 && mouse_y < 600)
                                stage=1;
                        }
                    }
                }
                break;

            case 7://CRÉDITOS
                while(stage==7)
                {
                    buttons = SDL_GetMouseState(&mouse_x, &mouse_y); //Adjunta unas coordenadas al mouse
                    imprimirImagen(window, tex.creditos);

                    while(SDL_PollEvent(&event))
                    {
                        if(event.type==SDL_QUIT) //Permite salir de la ventana si se cierra arriba a la derecha
                            return 0;

                            if(event.type==SDL_MOUSEBUTTONUP)
                        {
                            if ( mouse_x > 770 && mouse_x < 930 && mouse_y > 575 && mouse_y < 600)
                                stage=1;
                        }
                    }
                }
                break;
        }
    }

}


