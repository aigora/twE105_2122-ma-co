#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> //Preguntar si su uso est� permitido
#include <time.h> //Preguntar si su uso est� permitido
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_audio.h>
#include "Utilities.h"
#include "Tokens.h"
#include "Player.h"


void TokensCreator(Tokens Token[],Textures tex, Vector2f v[], int type, int ntokens)
{
    int i;
    Token[i].type = type;
    for(i=0; i<ntokens; i++)
    {
        Token[i].entity.src.x = 0;
        Token[i].entity.src.y = 0;
        Token[i].entity.dst.x = v[i].x;
        Token[i].entity.dst.y = v[i].y;

        switch(Token[i].type)
        {
        case 0://Taza café

            Token[i].entity.dst.w = 100;
            Token[i].entity.dst.h = 100;
            Token[i].entity.tex = tex.cafe;
            SDL_QueryTexture(tex.cafe,NULL, NULL, &Token[i].entity.src.w, &Token[i].entity.src.h);


        break;

        case 1://Coin

            Token[i].entity.dst.w = 100;
            Token[i].entity.dst.h = 100;
            Token[i].entity.tex = tex.billete;
            SDL_QueryTexture(tex.billete,NULL, NULL, &Token[i].entity.src.w, &Token[i].entity.src.h);
        break;

        case 2://Suelo  mojado

            Token[i].entity.dst.w = 100;
            Token[i].entity.dst.h = 100;
            Token[i].entity.tex = tex.charco;
            SDL_QueryTexture(tex.charco,NULL, NULL, &Token[i].entity.src.w, &Token[i].entity.src.h);
        break;
        }

        Token[i].collected=false;
    }
}

void renderToken(Tokens Token[], Window window, int ntokens)
{
    int i;
    for(i=0;i<ntokens;i++)
        if(Token[i].collected==false)
            SDL_RenderCopy(window.renderer, Token[i].entity.tex, &Token[i].entity.src, &Token[i].entity.dst);

}

void catchToken(Tokens Token[], int ntokens, player_t* player, Textures tex, Mix_Chunk *efecto, float gametime, int tiempo_fin_rap[1], int tiempo_fin_lent[1], int *velocidad, long long int *puntos)
{
    int gametime_int;
    gametime_int = (int) gametime;
    for(int i=0; i<ntokens; i++)
    {
        if(ColisionPlayer(*player, Token[i].entity)==1)
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
