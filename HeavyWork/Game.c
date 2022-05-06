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
    int nmuros=0, i, j, stage=1, last_time;
    float delta_time;

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

            last_time=SDL_GetTicks();

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
                                    movePlayer(player, muros, nmuros, MOVEMENT_UP,delta_time);
                                    break;
                                case SDL_SCANCODE_A:
                                case SDL_SCANCODE_LEFT:
                                    movePlayer(player, muros, nmuros, MOVEMENT_LEFT,delta_time);
                                    break;
                                case SDL_SCANCODE_S:
                                case SDL_SCANCODE_DOWN:
                                    movePlayer(player, muros, nmuros, MOVEMENT_DOWN,delta_time);
                                    break;
                                case SDL_SCANCODE_D:
                                case SDL_SCANCODE_RIGHT:
                                    movePlayer(player, muros, nmuros, MOVEMENT_RIGHT,delta_time);
                                    break;
                            }
                            break;
                    }

                }

                if(playerDist(player, bot)<=70)
                {
                    update = false;
                    game = false;
                }

                if(playerDist(player, bot)<=300)
                {
                    perseguir(player, bot, muros, nmuros, delta_time);
                }
                else
                    mov_bot (num_al(), bot, delta_time);


                printf("%.2f\n",1000.0/(SDL_GetTicks()-last_time));
                delta_time=(SDL_GetTicks()-last_time)/1000.0;
                last_time=SDL_GetTicks();
            }

            free(muros);
        }
    }
return 0;
}
