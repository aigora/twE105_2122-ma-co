#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_audio.h>

#include "Menu.h"
#include "Utilities.h"
#include "Font.h"

// Solo mostramos las 5 mejores puntuaciones
#define SCORE_LIST_SIZE 5
#define BOTON_ACEPTAR_W 200
#define BOTON_ACEPTAR_H 125


typedef struct {
    const char* filename;
    long long int score;
} score_file_t;

static void showScoreList(Window window, SDL_Texture* title) {
}

int menu(Window window, Textures tex, int *personaje, bool sonido, Mix_Music *musica)
{
    int stage=1;
    SDL_Event event;
    int mouse_x, mouse_y;
    int buttons;
    int cierre;

    Mix_PlayMusic( musica, -1 );//Reproduce la m�sica inicialmente
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
            {
                // Step 1: Leer todos los ficheros en "scores/".
                const char* path = "scores/";

                struct dirent *dp;
                DIR *dfd;

                if ((dfd = opendir(path)) == NULL) {
                    fprintf(stderr, "Can't open %s\n", path);
                    return 0;
                }

                char filename_qfd[100];
                score_file_t** scores = NULL;
                int num_scores = 0;
                while ((dp = readdir(dfd)) != NULL) {
                    struct stat stbuf;
                    sprintf(filename_qfd, "%s/%s", path, dp->d_name);
                    if (stat(filename_qfd, &stbuf) == -1) {
                        //printf("Unable to stat file: %s\n", filename_qfd);
                        continue;
                    }

                    if ((stbuf.st_mode & S_IFMT) == S_IFDIR) {
                        // Skip directories
                        continue;
                    } else {
                        //printf("File found - %s\n", filename_qfd);
                        FILE * fp;
                        fp = fopen(filename_qfd, "r");
                        if (fp == NULL) {
                            fprintf(stderr, "Can't open %s\n", filename_qfd);
                            return 0;
                        }

                        // 20 digitos, suficiente para representar un long long int.
                        char line[20] = { 0 };
                        char * end = NULL;
                        fgets(line, 20, fp);
                        long long int score = strtoll(line, &end, 10);

                        score_file_t* new_score = malloc(sizeof(score_file_t));
                        new_score->filename = malloc(strlen(dp->d_name) + 1);
                        strcpy(new_score->filename, dp->d_name);
                        new_score->score = score;

                        if (scores == NULL) {
                            num_scores++;
                            scores = malloc(sizeof(score_file_t*));
                        } else {
                            num_scores++;
                            scores = realloc(scores, sizeof(score_file_t*) * num_scores);
                        }

                        scores[num_scores - 1] = new_score;
                    }
                }

                for (int i = 0; i < num_scores; ++i) {
                    //printf("%s -> %d\n", scores[i]->filename, scores[i]->score);
                }

                // Step 2: Ordenarlos
                for (int i = 0; i < num_scores; ++i) {
                    for (int j = 1; j < num_scores; ++j) {
                        if (scores[j]->score > scores[j - 1]->score) {
                            score_file_t* temp = scores[j];
                            scores[j] = scores[j - 1];
                            scores[j - 1] = temp;
                        }
                    }
                }

                for (int i = 0; i < num_scores; ++i) {
                    //printf("%s -> %d\n", scores[i]->filename, scores[i]->score);
                }

                while (true) {
                    buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

                    while (SDL_PollEvent(&event)) {
                        switch (event.type) {
                            case SDL_QUIT:
                            {
                                // Liberar memoria
                                for (int i = 0; i < num_scores; ++i) {
                                    free(scores[i]->filename);
                                    free(scores[i]);
                                }
                                free(scores);

                                return 0;
                            }

                            case SDL_MOUSEBUTTONUP:
                            {
                                //printf("Click x=%d y=%d\n", mouse_x, mouse_y);
                                if (((mouse_x >= (window.w - BOTON_ACEPTAR_W - 50)) && (mouse_x <= (window.w - 50))) &&
                                    ((mouse_y >= (window.h - BOTON_ACEPTAR_H - 50)) && (mouse_y <= (window.h - 50))))
                                {
                                    // Se ha clickado en el boton de aceptar
                                    return 1;
                                }
                            } break;
                        }
                    }

                    // Step 3. Renderizar los 5 primeros.
                    SDL_RenderClear(window.renderer);

                    SDL_Color color = { .r=255, .g=255, .b=255, .a=255 };
                    font_texture_t title = newText("Puntuaciones", color, 28, window);

                    // Titulo
                    SDL_Rect title_rect;
                    title_rect.x = 50;
                    title_rect.y = 50;
                    title_rect.w = title.w;
                    title_rect.h = title.h;
                    SDL_RenderCopy(window.renderer, title.texture, NULL, &title_rect);

                    // Renderizar puntuaciones.
                    for (int i = 0; i < ((num_scores > 5) ? 5 : num_scores); ++i) {
                        char score_str[20];
                        sprintf(score_str, "%lld", scores[i]->score);

                        int digits = numDigits(scores[i]->score);

                        char* text = malloc(strlen(scores[i]->filename) + 2 + digits + 1);
                        strcpy(text, scores[i]->filename);
                        strcat(text, ": ");
                        strcat(text, score_str);

                        SDL_Color color = { .r=255, .g=255, .b=255, .a=255 };
                        font_texture_t score_texture = newText(text, color, 24, window);
                        free(text);

                        SDL_Rect title_rect;
                        title_rect.x = 50;
                        title_rect.y = 50 + 50 + title.h + 10 + (i * (score_texture.h + 5));
                        title_rect.w = score_texture.w;
                        title_rect.h = score_texture.h;
                        SDL_RenderCopy(window.renderer, score_texture.texture, NULL, &title_rect);
                    }

                    // Boton de aceptar
                    SDL_Rect button_rect;
                    button_rect.x = window.w - BOTON_ACEPTAR_W - 50;
                    button_rect.y = window.h - BOTON_ACEPTAR_H - 50;
                    button_rect.w = BOTON_ACEPTAR_W;
                    button_rect.h = BOTON_ACEPTAR_H;
                    SDL_RenderCopy(window.renderer, tex.boton, NULL, &button_rect);

                    SDL_RenderPresent(window.renderer);
                }

            } break;

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

            case 7://CR�DITOS
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


