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

int game(Window window, Textures tex, player_t* player, player_t* bot)
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

        case 1: //Generaci�n del laberinto

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
                renderPlayer(player, window);
                renderBot(bot,window);

                SDL_RenderPresent(window.renderer);

                //Updates
                while(SDL_PollEvent(&event))
                {
                    switch(event.type)
                    {
                        case SDL_QUIT:
                            update = false;
                            game = false;
                            break;

                        case SDL_KEYDOWN:

                            switch (event.key.keysym.scancode)
                            {
                                case SDL_SCANCODE_W:
                                case SDL_SCANCODE_UP:
                                    movePlayer(player, muros, nmuros, MOVEMENT_UP);
                                    break;
                                case SDL_SCANCODE_A:
                                case SDL_SCANCODE_LEFT:
                                    movePlayer(player, muros, nmuros, MOVEMENT_LEFT);
                                    break;
                                case SDL_SCANCODE_S:
                                case SDL_SCANCODE_DOWN:
                                    movePlayer(player, muros, nmuros, MOVEMENT_DOWN);
                                    break;
                                case SDL_SCANCODE_D:
                                case SDL_SCANCODE_RIGHT:
                                    movePlayer(player, muros, nmuros, MOVEMENT_RIGHT);
                                    break;
                            }
                            break;
                    }

                }

                mov_bot (num_al(), bot);
                if(playerDist(player, bot)<=100)
                {
                    update = false;
                    game = false;
                }
                SDL_Delay(1000/60); //Hay que cambiarlo por un temporizador de ejecuci�n
            }

            free(muros);
        }
    }
return 0;
}
