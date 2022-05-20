#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
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

    bool update,game=true, A_pres=false, boton=true, invisibilidad = false, invenc = false;
    bool pausa = false;
    SDL_Event event;
    M_Lab m_Lab;
    m_Lab.w=10;
    m_Lab.h=10;
    Entity *muros;
    key_buttons KEYS;KEYS.W=false;KEYS.A=false;KEYS.S=false;KEYS.D=false;KEYS.SPACE=false;KEYS.ESC=false;KEYS.ESC_PREV=false;
    int nmuros=0, i, j, stage=1, last_time;
    float delta_time,game_time,t_inicio,tiempo_boton_in,tiempo_boton_fin = 0,tiempo_fin_invisibilidad, tiempo_fin_invencibilidad;
    bool same_press;

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
                //Temporizador desde el comienzo del juego, se usa en la invisibilidad
                game_time = (SDL_GetTicks()-t_inicio)/1000.0;
                printf("%.2f\n",game_time);

                //Set positions etc

                //Dibujar la imagen
                if (!pausa) {
                    renderFondo(window,tex.fondo);
                    renderLab(window,muros,nmuros);
                    renderBot(bot,window);
                    renderPlayer(player, window, invisibilidad);
                    if (boton == false)
                    {
                        renderFondo(window, tex.vision);
                    }

                } else {
                    renderPause(window, tex.pause);
                }

                SDL_RenderPresent(window.renderer);

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
                        same_press = UpdateKeys(&KEYS,event,&game,&update);
                        break;
                    }
                }
                //! Esto por qué?
                // same_press = UpdateKeys(&KEYS, event, &game, &update);

                if (KEYS.ESC && !same_press) {
                    pausa = !pausa;
                    same_press = true;
                }

                // Si no esta el juego en pausa, podemos mover el jugador.
                if (!pausa) {
                    if (KEYS.W || KEYS.A || KEYS.S || KEYS.D) {
                        MovLab(muros, nmuros, KEYS, *player, bot, boton, delta_time);
                    }

                    if (KEYS.SPACE)
                    {
                        boton = boton_invisibilidad (boton, game_time, &tiempo_boton_in, &tiempo_boton_fin,&tiempo_fin_invisibilidad, &invisibilidad);
                    }

                    if (boton == false)
                    {
                        boton = boton_invisibilidad (boton, game_time, &tiempo_boton_in, &tiempo_boton_fin,&tiempo_fin_invisibilidad, &invisibilidad);
                    }
                    if((playerDist(player, bot, muros, nmuros)<=28)&&(invisibilidad == 0)&&(invenc == false))//Se le añade la condición de que no sea invisible para perseguir
                    {
                        bool alive = playerKill(player);
                        invenc = invencibilidad (game_time, &tiempo_fin_invencibilidad, invenc);

                        if (!alive) {
                            update = false;
                            game = false;
                        }
                    }

                    if((playerDist(player, bot, muros, nmuros)<=300)&&(invisibilidad == 0)&&(invenc == false))
                    {
                        perseguir(player, bot, muros, nmuros, delta_time, invisibilidad);
                    }
                    else
                        mov_bot (num_al(), bot, muros, nmuros, delta_time);

                    if (invenc == true) //Si invenc es verdadero se comprueba la igualdad de tiempos para, cuando se cumpla la condición, desactivarse
                    {
                        invenc = invencibilidad (game_time, &tiempo_fin_invencibilidad, invenc);
                    }
                }

                while(SDL_GetTicks()-last_time<1000/60){}
                printf("Frames: %.2f\n",1000.0/(SDL_GetTicks()-last_time));
                delta_time=(SDL_GetTicks()-last_time)/1000.0;
                last_time=SDL_GetTicks();
            }

            free(muros);
            free(player);
        }
    }
return 0;
}

bool invencibilidad (float time, int *tiempo_fin_invencibilidad, bool invenc)
{
    int t_ent,i;
    t_ent = (int) time;

    if (invenc == false)
    {
        *tiempo_fin_invencibilidad = t_ent+5;
        return true;
    }

    else if (t_ent == *tiempo_fin_invencibilidad)
    {
        return false;
    }

    else return true;
}
//La función invencibilidad sigue el mismo funcionamiento que la invisibilidad, al ser llamada cuando se pierde una vida,
//se hace verdadero el bool y se crea una variable tiempo_fin_invencibilidad. Esta es la suma del tiempo en el que se
//cambia el booleano y el tiempo que queremos que dure, 5 segundos. Se comprueba si el tiempo de juego en cada instante
//es el mismo que tiempo_fin_invencibilidad para entonces desactivar el bool.
