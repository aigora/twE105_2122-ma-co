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
    m_Lab.w=10;
    m_Lab.h=10;
    Entity *muros;
    key_buttons KEYS;KEYS.W=false;KEYS.A=false;KEYS.S=false;KEYS.D=false;
    int nmuros=0, i, j, stage=1, last_time, invisibilidad=0,aux_invisibilidad;
    float delta_time,game_time,t_inicio; //Las dos últimas se utilizan para un contador desde el inicio de juego

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
            t_inicio = SDL_GetTicks();

            while(update)
            {
                SDL_RenderClear(window.renderer);

                //Set positions etc

                //Dibujar la imagen
                renderFondo(window,tex.fondo);
                renderLab(window,muros,nmuros);
                renderPlayer(player, window);
                renderBot(bot,window);

                while(SDL_PollEvent(&event))
                {
                    switch(event.type)
                    {
                    case SDL_QUIT:
                        update=false;
                        game=false;
                        break;

                    case SDL_KEYDOWN:
                    case SDL_KEYUP:
                        UpdateKeys(&KEYS,event,&game,&update);
                        break;
                    }
                }
                UpdateKeys(&KEYS, event, &game, &update);


                SDL_RenderPresent(window.renderer);


                if(KEYS.W==true)
                    movePlayer(player, muros, nmuros, MOVEMENT_UP,delta_time);
                if(KEYS.A==true)
                    movePlayer(player, muros, nmuros, MOVEMENT_LEFT,delta_time);
                if(KEYS.S==true)
                    movePlayer(player, muros, nmuros, MOVEMENT_DOWN,delta_time);
                if(KEYS.D==true)
                    movePlayer(player, muros, nmuros, MOVEMENT_RIGHT,delta_time);



                //Temporizador desde el comienzo del juego, se usa en la invisibilidad
                game_time = (SDL_GetTicks()-t_inicio)/1000.0;
                printf("%.2f\n",game_time);

                //Invisibilidad, 1 activada, 0 desactivada
                invisibilidad = invisibility(game_time, &aux_invisibilidad, invisibilidad);
                /*if (invisibilidad == 1)
                    printf("Sí\n");
                else
                    printf("No\n");*/

                if((playerDist(player, bot, muros, nmuros)<=28)&&(invisibilidad == 0))//Se le añade la condición de que no sea invisible para perseguir
                {
                    // Reiniciar la posicion del jugador a la posicion inicial si ha chocado con el enemigo.
                    bool alive = playerKill(player);
                    if (!alive) {
                        update = false;
                        game = false;
                    }
                }

                if((playerDist(player, bot, muros, nmuros)<=300)&&(invisibilidad == 0))
                {
                    perseguir(player, bot, muros, nmuros, delta_time, invisibilidad);
                }
                else
                    mov_bot (num_al(), player, bot, muros, nmuros, delta_time);


                while(SDL_GetTicks()-last_time<1000/60){}
                printf("Frames: %.2f\n",1000.0/(SDL_GetTicks()-last_time));
                delta_time=(SDL_GetTicks()-last_time)/1000.0+1;
                last_time=SDL_GetTicks();

            }

            free(muros);
            free(player);

        }
    }
return 0;
}
