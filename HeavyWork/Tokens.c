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


void TokensCreator(Entity Token[],Textures tex, Vector2f v[], int type, int ntokens)
{
    int i;
    Vector2f v1;
    v1.x=v->x;
    v1.y=v->y;
    for(i=0; i<ntokens; i++)
    {
        Token[i].src.x = 0;
        Token[i].src.y = 0;
        Token[i].dst.x = v[i].x;
        Token[i].dst.y = v[i].y;

        switch(type)
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
            Token[i].tex = tex.cafe;
            SDL_QueryTexture(tex.cafe,NULL, NULL, &Token[i].src.w, &Token[i].src.h);
        break;

        case 2://Suelo  mojado

            Token[i].dst.w = 100;
            Token[i].dst.h = 100;
            Token[i].tex = tex.cafe;
            SDL_QueryTexture(tex.cafe,NULL, NULL, &Token[i].src.w, &Token[i].src.h);
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

void catchToken(Entity Token[], int ntokens, player_t* player, Textures tex)
{
    for(int i=0; i<ntokens; i++)
    {
        if(ColisionPlayer(*player, Token[i])==1)
        {
            Token[i].collected=true;
        }
    }
}
