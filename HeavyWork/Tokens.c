#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <direct.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_audio.h>
#include "Utilities.h"
#include "Tokens.h"
#include "Player.h"
#include <time.h>

void TokensCreator(Tokens Token[],Textures tex, Vector2i v[], int ncafe, int ndine, int ncharcos)
{
    int i, ntokens=ncafe+ndine+ncharcos;
    for(i=0; i<ntokens; i++)
    {
        Token[i].entity.src.x = 0;
        Token[i].entity.src.y = 0;
        Token[i].collected=false;
    }


    for(i=0;i<ncafe;i++)//Taza café
    {
        Token[i].entity.dst.w = 100;
        Token[i].entity.dst.h = 100;
        Token[i].entity.dst.x = v[i].x+80;
        Token[i].entity.dst.y = v[i].y+80;
        Token[i].entity.tex = tex.cafe;
        SDL_QueryTexture(tex.cafe,NULL, NULL, &Token[i].entity.src.w, &Token[i].entity.src.h);
        Token[i].type=0;
    }


    for(i=ncafe;i<ncafe+ndine;i++)//Coin
    {
        Token[i].entity.dst.w = 100;
        Token[i].entity.dst.h = 100;
        Token[i].entity.dst.x = v[i].x+130;
        Token[i].entity.dst.y = v[i].y;
        Token[i].entity.tex = tex.billete;
        SDL_QueryTexture(tex.billete,NULL, NULL, &Token[i].entity.src.w, &Token[i].entity.src.h);
        Token[i].type=1;
    }

    for(i=ncafe+ndine;i<ncafe+ndine+ncharcos;i++)//Suelo  mojado
    {
        Token[i].entity.dst.w = 100;
        Token[i].entity.dst.h = 100;
        Token[i].entity.dst.x = v[i].x+10;
        Token[i].entity.dst.y = v[i].y;
        Token[i].entity.tex = tex.charco;
        SDL_QueryTexture(tex.charco,NULL, NULL, &Token[i].entity.src.w, &Token[i].entity.src.h);
        Token[i].type=2;
    }


}

void renderToken(Tokens Token[], Window window, int ntokens)
{
    int i;
    for(i=0;i<ntokens;i++)
        if(Token[i].collected==false)
            SDL_RenderCopy(window.renderer, Token[i].entity.tex, &Token[i].entity.src, &Token[i].entity.dst);

}

void catchToken(Tokens Token[], int ntokens, player_t* player, Textures tex, Mix_Chunk *efecto, float gametime, int *tiempo_fin_rap, int *tiempo_fin_lent, int *velocidad, long long int *puntos)
{
    int gametime_int;
    gametime_int = (int) gametime;
    for(int i=0; i<ntokens; i++)
    {
        if(ColisionPlayer(*player, Token[i].entity)==1)
        {
            if (Token[i].collected==false)
            {
                Mix_PlayChannel( -1, efecto, 0 );//Reproduce efecto

            switch(Token[i].type)
            {
            case 0://Taza de café
                if (*velocidad == 1)
                {
                   *velocidad = 2; //Ultra velocidad
                   *tiempo_fin_rap = gametime_int + 5;
                }
                if (*velocidad == 2)
                {
                    *tiempo_fin_rap = *tiempo_fin_rap + 2;
                }
                else
                {
                    *velocidad = 1;
                }

                break;
            case 1://Moneda
                *puntos += 500; //Así suma 500 puntos por cada moneda
                //printf("%lli\n", *puntos);
                break;
            case 2:
                if (*velocidad == 1)
                {
                   *velocidad = 0;//Velocidad lenta
                   *tiempo_fin_lent = gametime_int + 5;
                }
                if (*velocidad == 0)
                {
                    *tiempo_fin_lent = *tiempo_fin_lent + 2;
                }
                else
                    *velocidad = 1;

                break;
            }
            }
            Token[i].collected=true;
        }
    }
}

void writeScoreToFile(const char* filename, long long int score) {
    printf("write file %s...\n", filename);
    int ret = _mkdir("scores");

    char *path = malloc(strlen(filename) + 6 + 1); // +1 for the null-terminator
    strcpy(path, "scores/");
    strcat(path, filename);

    FILE* fd = fopen(path, "w");
    if (fd != NULL) {
        fprintf(fd, "%d", score);
        printf("success!\n");
    } else {
        printf("error!\n");
    }

    free(path);
    fclose(fd);
}
