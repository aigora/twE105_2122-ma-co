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
#include "Font.h"

#define BOTON_ACEPTAR_W 200
#define BOTON_ACEPTAR_H 125

int game(Window window, Textures tex, Mix_Chunk *recoger, Mix_Chunk *invisi)
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
    int nmuros, i, j, stage=1, last_time, nbots, velocidad = 1, ncharcos ,ncafe, ndine, ntok;
    int tiempo_fin_rap= 0, tiempo_fin_lent = 0;
    int vidas;
    long long int puntos=1000;
    float delta_time,game_time,tiempo_boton_in = 0,tiempo_boton_fin = 0,tiempo_fin_invisibilidad = 0,tiempo_fin_invencibilidad = 0;//Todos los contadores utilizados para finalizar los superpoderes
    bool same_press;
    Tokens *tok;
    Vector2i inip,inis,desfase;

    player_textures_t player_textures = {
        .player_right = tex.playerdrcha,
        .player_left = tex.playerizqda,
        .player_up = tex.playeratras,
        .player_down = tex.player,
        .player_inv_right = tex.playerinvdrcha,
        .player_inv_left = tex.playerinvizqda,
        .player_inv_up = tex.playerinvatras,
        .player_inv_down = tex.playerinv,
        .life = tex.vida,
    };


    while(game)
    {
        switch(stage)
        {
        case 0:
            return 0;
            break;

        case 1: //Generaci�n del nivel

            //Semilla aleatoria de generación
            srand(time(NULL));

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

            drawLab(window,m_Lab,muros,tex);

            //Generación del personage
            vidas=5;
            inip.x=rand() % m_Lab.w;
            inip.y=rand() % m_Lab.h;
            inip.x=1;
            inip.y=0;

            player_t* player = newPlayer(inip, vidas, player_textures,window,&desfase,m_Lab.w,m_Lab.h);

            //Generación de salida
            generarSalida(&salida,m_Lab,inip,tex,&inis,desfase);

            //Generación de los bots
            nbots=10;

            bots=malloc(nbots*sizeof(Bot));
            if(bots==NULL)
            {
                printf("Error creando bots");
                exit(-1);
            }

            generarBots(m_Lab,bots,nbots,tex,inip,desfase);

            //Generación de los coleccionables(Tokens)
            ncafe=10;
            ncharcos=10;
            ndine=10;
            ntok=ncafe+ncharcos+ndine;

            tok=malloc(ntok*sizeof(Tokens));
            if(tok==NULL)
            {
                printf("Error creando tokens");
                exit(-1);
            }

            generarTokens(m_Lab,tok,ncafe,ndine,ncharcos,tex,inip,inis,desfase);

            //Desfasa los muros para centrar la camara en el personaje
            desfasarMuros(muros,nmuros,desfase);

            //Salida de la generación
            free(m_Lab.esq);
            stage=2;
            update=true;

            break;

        case 2: //Bucle del juego

            last_time=SDL_GetTicks();


            while(update)
            {
                SDL_RenderClear(window.renderer);
                //Temporizador desde el comienzo del juego, se usa en la invisibilidad
                game_time += delta_time;

                if (!pausa)
                {
                    renderFondo(window,tex.fondo);
                    renderLab(window,muros,nmuros,salida);
                    renderToken(tok,window,ntok);
                    renderBot(bots,window,nbots);
                    renderPlayer(player, window, invisibilidad);
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
                        movement(window,muros, &salida, nmuros, KEYS, player, bots, tok, ntok, nbots, boton, delta_time, velocidad);
                        catchToken(tok, ntok, player, tex, recoger,game_time, &tiempo_fin_rap, &tiempo_fin_lent, &velocidad, &puntos);
                    }
                    //printf("%i\n", tiempo_fin_lent);
                    //printf("%i\n", tiempo_fin_rap);
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

                    if (invenc == true) //Comprueba si ya ha pasado el tiempo de invencibilidad (5s)
                        invenc = invencibilidad (game_time, &tiempo_fin_invencibilidad, invenc);

                    if (velocidad != 1)//Comprueba si ya ha pasado el tiempo de rapidez/lentitud (5s)
                        velocidad =  finvelo (game_time, 1, tiempo_fin_rap, tiempo_fin_lent, velocidad);
                        for(i=0;i<nbots;i++)
                    if((playerDist(player, bots[i], muros, nmuros)<=28)&&(invisibilidad == 0)&&(invenc == false))
                    {
                        // Reiniciar la posicion del jugador a la posicion inicial si ha chocado con el enemigo.
                        bool alive = playerKill(player);
                        invenc = invencibilidad (game_time, &tiempo_fin_invencibilidad, invenc);

                        if (!alive)
                        {
                            update = false;
                            game = true;
                        }
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
            free(tok);
            stage = 0;
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

int finvelo (float gametime, int ntokens, int tiempo_fin_rap, int tiempo_fin_lent, int velocidad)
{
    int gametime_int,i;
    gametime_int = (int) gametime;
    for (i=0;i<ntokens;i++)
    {
       if ((gametime_int == tiempo_fin_rap)&&(velocidad == 2))
       {
           return 1;
       }
       else if ((velocidad == 2))
       {
           return 2;
       }
       else if ((gametime_int == tiempo_fin_lent)&&(velocidad == 0))
       {
           return 1;
       }
       else return 0;
    }

}

static void renderScoreScreen(Window window, SDL_Texture* text, SDL_Texture* boton_aceptar, font_texture_t tex_filename) {
    SDL_RenderClear(window.renderer);
    // Titulo
    SDL_Rect title_rect;
    title_rect.x = 50;
    title_rect.y = 50;
    title_rect.w = 300;
    title_rect.h = 100;
    SDL_RenderCopy(window.renderer, text, NULL, &title_rect);

    // Boton de aceptar
    SDL_Rect button_rect;
    button_rect.x = window.w - BOTON_ACEPTAR_W - 50;
    button_rect.y = window.h - BOTON_ACEPTAR_H - 50;
    button_rect.w = BOTON_ACEPTAR_W;
    button_rect.h = BOTON_ACEPTAR_H;
    SDL_RenderCopy(window.renderer, boton_aceptar, NULL, &button_rect);

    // Input box
    SDL_Rect input_background;
    input_background.x = 50;
    input_background.y = 200;
    input_background.w = window.w - 100;
    input_background.h = 60;
    SDL_SetRenderDrawColor(window.renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(window.renderer, &input_background);
    SDL_SetRenderDrawColor(window.renderer, 0, 0, 0, 255);

    // Filename
    SDL_Rect filename_rect;
    filename_rect.x = 50 + 10;
    filename_rect.y = 200 + 20;
    filename_rect.w = tex_filename.w;
    filename_rect.h = tex_filename.h;
    SDL_RenderCopy(window.renderer, tex_filename.texture, NULL, &filename_rect);

    SDL_RenderPresent(window.renderer);
}

int exitScreen(Window window, Textures tex, long long int score) {
    bool exit = false;
    char* filename = NULL;
    int length = 1;
    int mouse_x;
    int mouse_y;
    SDL_Event event;
    font_texture_t text_filename;

    while (!exit) {
        SDL_GetMouseState(&mouse_x, &mouse_y);
        while (SDL_PollEvent(&event)) {
            switch (event.type)
            {
                case SDL_QUIT:
                {
                    exit = true;
                } break;

                case SDL_KEYDOWN:
                {
                    char character = 0;
                    if ((event.key.keysym.scancode >= SDL_SCANCODE_A) &&
                        (event.key.keysym.scancode <= SDL_SCANCODE_Z))
                    {
                        character = event.key.keysym.scancode + 61;
                    }
                    else if ((event.key.keysym.scancode >= SDL_SCANCODE_1) &&
                             (event.key.keysym.scancode <= SDL_SCANCODE_9))
                    {
                        character = event.key.keysym.scancode + 19;
                    } else if (event.key.keysym.scancode >= SDL_SCANCODE_0)
                    {
                        character = '0';
                    }

                    printf("Tecla presionada = %c\n", character);
                    length += sizeof(char);
                    if (filename == NULL) {
                        // Reservamos espacio para dos char (la letra que queremos guardar, más el null-terminator)
                        filename = malloc(sizeof(char) * 2);
                    } else {
                        filename = realloc(filename, length + sizeof(char));
                    }

                    filename[length - 2] = character;
                    filename[length - 1] = '\0';

                    SDL_Color colour = { .r=0, .g=0, .b=0, .a=255 };
                    text_filename = newText(filename, colour, 20, window);

                } break;

                case SDL_MOUSEBUTTONUP:
                {
                    printf("Click x=%d y=%d\n", mouse_x, mouse_y);
                    if (((mouse_x >= (window.w - BOTON_ACEPTAR_W - 50)) && (mouse_x <= (window.w - 50))) &&
                        ((mouse_y >= (window.h - BOTON_ACEPTAR_H - 50)) && (mouse_y <= (window.h - 50))))
                    {
                        // Se ha clickado en el boton de aceptar
                        writeScoreToFile(filename, score);
                        exit = true;
                    }
                } break;
            }
        }

        renderScoreScreen(window, tex.titulo_puntuacion, tex.boton, text_filename);
        // SDL_DestroyTexture(text_filename.texture);
    }

    free(filename);

    return 0;
}
