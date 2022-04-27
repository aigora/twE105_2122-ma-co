#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_audio.h>
#include "Game.h"
#include "Player.h"
#include "Utilities.h"
#include "Laberinto.h"

int game(Window window, Textures tex, int personaje)
{
    SDL_RenderClear(window.renderer);
    SDL_RenderCopy(window.renderer, tex.carga, NULL, NULL);
    SDL_RenderPresent(window.renderer);

    bool update,game=true, A_pres=false;
    SDL_Event event;
    M_Lab m_Lab;
    m_Lab.w=4;
    m_Lab.h=4;
    Entity *muros;
    int nmuros=0, i, j, stage=1;

    while(game)
    {
        switch(stage)
        {
        case 0:
            return 0;

        case 1: //Generación del laberinto

            m_Lab.esq = malloc((m_Lab.w*2+1)*(m_Lab.h*2+1)*sizeof(char));
            if(m_Lab.esq==NULL)
            {
                printf("Error reservando memoria");
                exit(-1);
            }
            generarLaberinto(m_Lab);
            //DebugLab(m_Lab);

            nmuros=0;
            for(j=1;j<m_Lab.h*2;j++)
            {
                for(i=1;i<m_Lab.w*2;i++)
                {
                    if((((i%2!=0)&&(j%2==0))||(j%2!=0))&&(m_Lab.esq[i+j*(2*m_Lab.w+1)]=='#'))
                        nmuros++;
                }
            }
            nmuros+=4;

            muros=malloc((nmuros+4)*sizeof(Entity));
            drawLab(window,m_Lab,muros,tex.wall);
            free(m_Lab.esq);
            stage=2;
            update=true;

            break;

        case 2: //Bucle del juego

            while(update)
            {
                SDL_RenderClear(window.renderer);

                //Set positions etc

                //Dibujar la imagen
                renderFondo(window,tex.fondo);
                renderLab(window,muros,nmuros);
                SDL_RenderPresent(window.renderer);

                //Updates
                while(SDL_PollEvent(&event))
                {
                    if(event.type==SDL_QUIT)
                    {
                        free(muros);
                        return 0;
                    }
                }

                SDL_Delay(1000/60); //Hay que cambiarlo por un temporizador de ejecución
            }
            free(muros);

        }
    }

}
