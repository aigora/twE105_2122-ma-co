#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_audio.h>
#include "Game.h"
#include "Player.h"
#include "Utilities.h"
#include "Laberinto.h"
#include "Tokens.h"

int game(Window window, Textures tex, player_t* player, Mix_Chunk *recoger, Mix_Chunk *invisi)
{
    SDL_RenderClear(window.renderer);
    SDL_RenderCopy(window.renderer, tex.carga, NULL, NULL);
    SDL_RenderPresent(window.renderer);

    bool update,game=true, A_pres=false, boton=true, invisibilidad = false, invenc = false;
    bool pausa = false;
    SDL_Event event;
    M_Lab m_Lab;
    Entity *muros;
    Entity salida;
    Bot* bots;
    key_buttons KEYS;KEYS.W=false;KEYS.A=false;KEYS.S=false;KEYS.D=false;KEYS.SPACE=false;KEYS.ESC=false;KEYS.ESC_PREV=false;
    int nmuros=0, i, j, stage=1, last_time, nbots, velocidad = 1;
    int tiempo_fin_rap[1]= {0}, tiempo_fin_lent[1] = {0};
    long long int puntos=1000;
    float delta_time,game_time,tiempo_boton_in = 0,tiempo_boton_fin = 0,tiempo_fin_invisibilidad = 0,tiempo_fin_invencibilidad = 0;
    bool same_press;
    Entity Tok[1];
    Vector2f v[1];v[0].x=100;v[0].y=100;//!Borrar esto cuando añadamos más tokens

    while(game)
    {
        switch(stage)
        {
        case 0:
            return 0;

        case 1: //Generaci�n del laberinto

            m_Lab.w=10;
            m_Lab.h=10;
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
            if(muros==NULL)
            {
                printf("Error creando muros");
                exit(-1);
            }

            drawLab(window,m_Lab,muros,&salida,tex);

            //Generación de los bots y coleccionables(Tokens)
            nbots=10;

            bots=malloc(nbots*sizeof(Bot));
            if(bots==NULL)
            {
                printf("Error creando bots");
                exit(-1);
            }

            generarBots(m_Lab,bots,nbots,tex);

            //Salida de la generación
            free(m_Lab.esq);
            stage=2;
            update=true;
            TokensCreator(Tok,tex, v, 0, 1); //Tal y como está, al recoger la taza aumenta la velocidad.Si donde pone 0 se pone 1, imprime
            //"¡100 ptos!" al recoger el billete, si se pone tipo 1 spawnea el charco y al pasar por él ralentiza durante los 5s

            break;

        case 2: //Bucle del juego

            last_time=SDL_GetTicks();


            while(update)
            {
                SDL_RenderClear(window.renderer);
                //Temporizador desde el comienzo del juego, se usa en la invisibilidad
                game_time += delta_time;
                //printf("%.2f\n",game_time);

                if (!pausa)
                {
                    renderFondo(window,tex.fondo);
                    renderLab(window,muros,nmuros,salida);
                    renderToken(Tok,window,1);
                    renderBot(bots,window,nbots);
                    renderPlayer(player, window, invisibilidad);
                    if (boton == false)
                        renderFondo(window, tex.vision);
                }
                else
                    renderPause(window, tex.pause);


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

                if (KEYS.ESC && !same_press) {
                    pausa = !pausa;
                    same_press = true;
                }

                // Si no esta el juego en pausa, podemos mover el jugador.
                if (!pausa)
                {
                    if (KEYS.W || KEYS.A || KEYS.S || KEYS.D)
                    {
                        player_direction_t direction;
                        if (KEYS.W) {
                            direction = MOVEMENT_UP;
                        } else if (KEYS.A) {
                            direction = MOVEMENT_LEFT;
                        } else if (KEYS.S) {
                            direction = MOVEMENT_DOWN;
                        } else if (KEYS.D) {
                            direction = MOVEMENT_RIGHT;
                        }

                        playerSetDirection(player, direction);
                        movLab(muros, &salida, nmuros, KEYS, *player, bots, Tok, 1, nbots, boton, delta_time, velocidad);
                        catchToken(Tok, 1, player, tex, recoger,game_time, tiempo_fin_rap, tiempo_fin_lent,&velocidad, &puntos);
                    }
                    //printf("%i\n",velocidad);
                    //printf("%i\n",tiempo_fin_rap[1]);
                    //printf("%i\n",tiempo_fin_lent[1]);
                    //printf("%lli\n", puntos);
                    if (KEYS.SPACE)
                        boton = boton_invisibilidad (boton, game_time, &tiempo_boton_in, &tiempo_boton_fin,&tiempo_fin_invisibilidad, &invisibilidad, invisi);

                    if (boton == false)
                        boton = boton_invisibilidad (boton, game_time, &tiempo_boton_in, &tiempo_boton_fin,&tiempo_fin_invisibilidad, &invisibilidad, invisi);

                    for(i=0;i<nbots;i++)
                    if((playerDist(player, bots[i], muros, nmuros)<=28)&&(invisibilidad == 0)&&(invenc == false))
                    {
                        // Reiniciar la posicion del jugador a la posicion inicial si ha chocado con el enemigo.
                        bool alive = playerKill(player);
                        invenc = invencibilidad (game_time, &tiempo_fin_invencibilidad, invenc);

                        if (!alive)
                        {
                            update = false;
                            game = false;
                        }
                    }

                    for(i=0;i<nbots;i++)
                    {
                    if((playerDist(player, bots[i], muros, nmuros)<=300)&&(invisibilidad == 0)&&(invenc==false))
                    {
                        perseguir(player, &bots[i], muros, nmuros, delta_time, invisibilidad);
                    }
                    else
                        mov_bot (num_al(), &bots[i], muros, nmuros, delta_time);
                    }

                    if (invenc == true) //Comprueba si ya han pasado el tiempo de invencibilidad
                        invenc = invencibilidad (game_time, &tiempo_fin_invencibilidad, invenc);
                    if (velocidad != 1)
                    {
                        velocidad =  finvelo (game_time, 1, tiempo_fin_rap, tiempo_fin_lent, velocidad);
                    }

                }

                while(SDL_GetTicks()-last_time<1000/60){}
                //printf("FPS: %.2f\n",1000.0/(SDL_GetTicks()-last_time));
                delta_time=(SDL_GetTicks()-last_time)/1000.0;
                last_time=SDL_GetTicks();
            }

            free(muros);
            free(player);
            free(bots);
            stage = 3;
            break;
        }
    }

    return stage;
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
        return false;

    else return true;
}
//La función invencibilidad sigue el mismo funcionamiento que la invisibilidad, al ser llamada cuando se pierde una vida,
//se hace verdadero el bool y se crea una variable tiempo_fin_invencibilidad. Esta es la suma del tiempo en el que se
//cambia el booleano y el tiempo que queremos que dure, 5 segundos. Se comprueba si el tiempo de juego en cada instante
//es el mismo que tiempo_fin_invencibilidad para entonces desactivar el bool.

int finvelo (float gametime, int ntokens, int tiempo_fin_rap[], int tiempo_fin_lent[], int velocidad)
{
    int gametime_int,i;
    gametime_int = (int) gametime;
    for (i=0;i<ntokens;i++)
    {
       if ((gametime_int == tiempo_fin_rap[i])&&(velocidad == 2))
       {
           return 1;
       }
       else if ((velocidad == 2))
       {
           return 2;
       }
       else if ((gametime_int == tiempo_fin_lent[i])&&(velocidad == 0))
       {
           return 1;
       }
       else return 0;
    }

}

int exitScreen(Window window, Textures tex, long long int score) {
    while (true) {
        renderScoreScreen(window, tex.titulo_puntuacion);
    }

    return 0;
}
