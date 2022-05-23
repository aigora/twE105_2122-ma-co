#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> //Preguntar si su uso est� permitido
#include <time.h> //Preguntar si su uso est� permitido
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_audio.h>
#include "Utilities.h"
#include "Tokens.h"
#include "Player.h"
#include <time.h>

void TokensCreator(Entity Token[],Textures tex, Vector2f v[], int type, int ntokens)
{
    int i;
    Token[i].type = type;
    for(i=0; i<ntokens; i++)
    {
        Token[i].src.x = 0;
        Token[i].src.y = 0;
        Token[i].dst.x = v[i].x;
        Token[i].dst.y = v[i].y;

        switch(Token[i].type)
        {
        case 0://Taza café

            Token[i].dst.w = 100;
            Token[i].dst.h = 100;
            Token[i].tex = tex.cafe;
            SDL_QueryTexture(tex.cafe,NULL, NULL, &Token[i].src.w, &Token[i].src.h);


        break;

        case 1://Coin

            Token[i].dst.w = 100;
            Token[i].dst.h = 100;
            Token[i].tex = tex.billete;
            SDL_QueryTexture(tex.billete,NULL, NULL, &Token[i].src.w, &Token[i].src.h);
        break;

        case 2://Suelo  mojado

            Token[i].dst.w = 100;
            Token[i].dst.h = 100;
            Token[i].tex = tex.charco;
            SDL_QueryTexture(tex.charco,NULL, NULL, &Token[i].src.w, &Token[i].src.h);
        break;
        }

        Token[i].collected=false;
    }
}

void renderToken(Entity Token[], Window window, int ntokens)
{
    int i;
    for(i=0;i<ntokens;i++)
        if(Token[i].collected==false)
            SDL_RenderCopy(window.renderer, Token[i].tex, &Token[i].src, &Token[i].dst);

}

void catchToken(Entity Token[], int ntokens, player_t* player, Textures tex, Mix_Chunk *efecto, float gametime, int tiempo_fin_rap[1], int tiempo_fin_lent[1], int *velocidad, long long int *puntos)
{
    int gametime_int;
    gametime_int = (int) gametime;
    for(int i=0; i<ntokens; i++)
    {
        if(ColisionPlayer(*player, Token[i])==1)
        {
            if (Token[i].collected==false)
            {
                Mix_PlayChannel( -1, efecto, 0 );

            switch(Token[i].type)
            {
            case 0://Taza de café
                *velocidad = 2; //Ultra velocidad
                tiempo_fin_rap[i] = gametime_int + 5;
                break;
            case 1://Moneda
                *puntos += 500; //Así suma 500 puntos por cada moneda
                break;
            case 2:
                *velocidad = 0;//Velocidad lenta
                tiempo_fin_lent[i] = gametime_int + 5;
                break;
            }
            }
            Token[i].collected=true;
        }
    }
}

void renderScoreScreen(Window window, SDL_Texture* text) {
    SDL_RenderClear(window.renderer);
    SDL_Rect title_rect;
    title_rect.x = 50;
    title_rect.y = 50;
    title_rect.w = 300;
    title_rect.h = 100;
    SDL_RenderCopy(window.renderer, text, NULL, &title_rect);

    SDL_Rect input_background;
    input_background.x = 50;
    input_background.y = 200;
    input_background.w = 300;
    input_background.h = 100;
    SDL_SetRenderDrawColor(window.renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(window.renderer, &input_background);
    SDL_SetRenderDrawColor(window.renderer, 0, 0, 0, 255);

    SDL_RenderPresent(window.renderer);
}

static void formatFilename(char *output) {
    time_t rawtime;
    struct tm * timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    sprintf(output, "score_%d%d%d-%d%d%d.txt", timeinfo->tm_mday,
            timeinfo->tm_mon + 1, timeinfo->tm_year + 1900,
            timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
}

void writeScoreToFile(long long int score) {
    char* filename = malloc(64);
    formatFilename(filename);
    FILE* fd = fopen(filename, "w");
    if (fd != NULL) {
        fprintf(fd, "Score: %d\n", score);
    }

    free(filename);
    fclose(fd);
}
