#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_audio.h>
#include "Menu.h"
#include "Utilities.h"
#define SPEED 300//velocidad en pixeles por segundo

int menu(Window window, Textures tex, int *personaje, int sonido)
{
    int stage=1;
    SDL_Event event; //Creamos una variable de tipo evento
    int mouse_x, mouse_y;
    int buttons;
    int cierre;
    // Para cargar un archivo de audio, el formato reconocido por la librería básica de SDL es WAV. El clip de audio es cargado:
    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8 *wavBuffer;

    SDL_LoadWAV("resources/music2.wav", &wavSpec, &wavBuffer, &wavLength);

    // Con el siguiente comando se abre el dispositivo de audio que lo va a reproducir

    SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);

    // Los argumentos del comando anterior hacen referencia, el primero al nombre del dispositivo que lo reproducirá (al pasarle NULL
    // tomará el dispositivo predeterminado, el segundo es relevante en relación a dispositivos de grabación, no de reproducción, el
    // tercero representa el formato del audio de entrada, el cuarto el formato del audio de salida y el quinto se refiere a escenarios avanzados de audio

    // Reproducir la pista

    int success = SDL_QueueAudio(deviceId, wavBuffer, wavLength); // SDL_QueueAudio permite enviar la información del WAV directamente al dispositivo
    int SDL_CaptureMouse(SDL_bool enabled);//Relativo a la detección del ratón

    while(stage!=0)
    {
        if(sonido==0)
        {
            SDL_PauseAudioDevice(deviceId, 0); //Pausa la grabación de audio al darle un número != 0
        }
        if(sonido==1)
        {
            SDL_PauseAudioDevice(deviceId, 1);
        }

        //Bucle del menú

        switch (stage)
        {
        case 1:
        {
            while(stage==1)
            {
                imprimirImagen(window, tex.menu);
                buttons = SDL_GetMouseState(&mouse_x, &mouse_y); //Adjunta unas coordenadas al mouse

                while(SDL_PollEvent(&event))
                {
                    if(event.type==SDL_QUIT) //Permite salir de la ventana si se cierra arriba a la derecha
                        return 0;
                    if(event.type==SDL_MOUSEBUTTONUP)
                    {
                        if(mouse_x < 450 && mouse_y > 420 && mouse_x > 120 && mouse_y < 500) //JUGAR
                        {
                            SDL_RenderClear(window.renderer);
                            SDL_RenderCopy(window.renderer, tex.carga, NULL, NULL);
                            SDL_RenderPresent(window.renderer);
                            return 2;
                        }
                        if(mouse_x > 550 && mouse_y > 420 && mouse_x < 880 && mouse_y < 500) //SCORES
                        {
                            stage = 3;
                        }
                        if(mouse_x > 120 && mouse_x < 450 && mouse_y > 520 && mouse_y < 600) //AJUSTES
                        {
                            stage = 4;
                        }
                        if (mouse_x > 550 && mouse_x < 880 && mouse_y > 520 && mouse_y < 600) //Salir
                        {
                            return 0;
                        }
                    }
                }
            }
        }
        case 3://SCORES
            while(stage==3)
            {
                imprimirImagen(window, tex.prox);

                buttons = SDL_GetMouseState(&mouse_x, &mouse_y); //Adjunta unas coordenadas al mouse

                while(SDL_PollEvent(&event))
                {
                    if(event.type==SDL_QUIT) //Permite salir de la ventana si se cierra arriba a la derecha
                    {
                        return 0;
                    }
                }

            }

            break;
        case 4://AJUSTES
            while(stage==4)
            {
                buttons = SDL_GetMouseState(&mouse_x, &mouse_y); //Adjunta unas coordenadas al mouse

                if (sonido == 0)
                {
                    imprimirImagen(window, tex.ajustes);
                }
                if (sonido == 1)
                {
                    imprimirImagen(window, tex.ajusnos);
                }

                while(SDL_PollEvent(&event))
                {
                    if(event.type==SDL_QUIT) //Permite salir de la ventana con la cruceta
                    {
                        return 0;
                    }
                    if(event.type==SDL_MOUSEBUTTONUP)
                    {
                        if(mouse_x < 290 && mouse_y > 450 && mouse_x > 60 && mouse_y < 540)//SONIDO
                        {
                            if (sonido == 1)
                            {
                                sonido=0;
                                SDL_PauseAudioDevice(deviceId, 0);
                            }
                            else
                            {
                                sonido=1;
                                SDL_PauseAudioDevice(deviceId, 1);
                            }
                        }
                        if(mouse_x > 400 && mouse_y > 450 && mouse_x < 630 && mouse_y < 540) //Selecciona en que parte de la pantalla puedo clickar
                        {
                            stage=6;
                        }
                        if ( mouse_x > 710 && mouse_x < 900 && mouse_y > 450 && mouse_y < 540)
                        {
                            stage=7;
                        }
                        if ( mouse_x > 830 && mouse_x < 980 && mouse_y > 575 && mouse_y < 600)
                        {
                            stage=1;
                        }
                    }
                }
            }
            break;
        case 6://PERSONAJE
            while(stage==6)
            {
                if (*personaje == 1)
                {
                    imprimirImagen(window, tex.personaje1);
                }
                else if (*personaje == 2)
                {
                    imprimirImagen(window, tex.personaje2);
                }
                else
                {
                    imprimirImagen(window, tex.personaje3);
                }

                buttons = SDL_GetMouseState(&mouse_x, &mouse_y); //Adjunta unas coordenadas al mouse

                while(SDL_PollEvent(&event))
                {
                    if(event.type==SDL_QUIT) //Permite salir de la ventana si se cierra arriba a la derecha
                    {
                        return 0;
                    }
                    if(event.type==SDL_MOUSEBUTTONUP) //PRIMER PERSONAJE
                    {
                        if ( mouse_x > 130 && mouse_x < 315 && mouse_y > 275 && mouse_y < 520)
                        {
                            *personaje = 1;
                        }
                        if ( mouse_x > 390 && mouse_x < 570 && mouse_y > 275 && mouse_y < 520)
                        {
                            *personaje = 2;
                        }
                        if ( mouse_x > 640 && mouse_x < 820 && mouse_y > 275 && mouse_y < 520)
                        {
                            *personaje = 3;
                        }

                        if ( mouse_x > 830 && mouse_x < 980 && mouse_y > 575 && mouse_y < 600)
                        {
                            stage=1;
                        }
                    }

                }

            }


            break;
        case 7://USUARIO
            while(stage==7)
            {
                buttons = SDL_GetMouseState(&mouse_x, &mouse_y); //Adjunta unas coordenadas al mouse

                imprimirImagen(window, tex.prox);

                while(SDL_PollEvent(&event))
                {
                    if(event.type==SDL_QUIT) //Permite salir de la ventana si se cierra arriba a la derecha
                    {
                        return 0;
                    }
                }
            }
            break;
        }
    }
}


