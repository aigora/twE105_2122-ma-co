#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "Laberinto.h"
#include "Utilities.h"
#include "Enemies.h"
#include "Tokens.h"

void generarLaberinto(M_Lab m_Lab)
{
    int i, j, r, visited, posible=0, N=m_Lab.h*m_Lab.w, last, lTOi, dirList[4];


    Cell *cell_info; //Variable que contiene la información de cada celda (vacia los muros se dibujaran despues con esta información)

    Stack *stack; //Variable que contiene el recorrido aleatorio

    cell_info = malloc(N*sizeof(Cell)); //Asignamos el tamaño del laberinto
    if(cell_info==NULL)
    {
        printf("Error reservando memoria");
        exit(-1);
    }

    stack = malloc(N*sizeof(Stack)); //Asignamos el mayor número que el stack tendrá que almacenar dado el caso
    if(stack==NULL)
    {
        printf("Error reservando memoria");
        exit(-1);
    }

    //Pone todas las variables iniciadas a false
    for(i=0;i<N;i++)
    {
        cell_info[i].visited=false;
        cell_info[i].east=false;
        cell_info[i].west=false;
        cell_info[i].north=false;
        cell_info[i].south=false;
    }

    //Empieza en la celda (0,0) (Se puede modificar para que podamos escojar en la celda que empieza)
    i=0;
    j=0;
    cell_info[i+j*m_Lab.w].visited=true;

    stack[0].x=i;
    stack[0].y=j;
    stack[0].last=true;
    visited=1;

    do
    {
        posible=0;

        //Comprobamos a que casillas podemos ir
        //Oeste
        if(i>0 && (cell_info[i+j*m_Lab.w-1].visited==false))
        {
            dirList[posible]=0;
            posible++;
            //printf("Posible ir a Oeste \n");
        }

        //Norte
        if(j>0 && (cell_info[i+(j-1)*m_Lab.w].visited==false))
        {
            dirList[posible]=1;
            posible++;
            //printf("Posible ir a Norte \n");
        }

        //Este
        if(i<m_Lab.w-1 && (cell_info[i+j*m_Lab.w+1].visited==false))
        {
            dirList[posible]=2;
            posible++;
            //printf("Posible ir a Este \n");
        }

        //Sur
        if(j<m_Lab.h-1 && (cell_info[i+(j+1)*m_Lab.w].visited==false))
        {
            dirList[posible]=3;
            posible++;
            //printf("Posible ir a Sur \n");
        }

        if(posible!=0)
        {
            //Decide cual es la siguiente dirección que toma de manera aleatoria
            r=rand() % posible;

            switch (dirList[r])
            {
            case 0:
                cell_info[i+j*m_Lab.w].west=true;
                i=i-1;
                cell_info[i+j*m_Lab.w].east=true;
                cell_info[i+j*m_Lab.w].visited=true;
                pushStack(stack,i,j);
                break;

            case 1:
                cell_info[i+j*m_Lab.w].north=true;
                j=j-1;
                cell_info[i+j*m_Lab.w].south=true;
                cell_info[i+j*m_Lab.w].visited=true;
                pushStack(stack,i,j);
                break;

            case 2:
                cell_info[i+j*m_Lab.w].east=true;
                i=i+1;
                cell_info[i+j*m_Lab.w].west=true;
                cell_info[i+j*m_Lab.w].visited=true;
                pushStack(stack,i,j);
                break;

            case 3:
                cell_info[i+j*m_Lab.w].south=true;
                j=j+1;
                cell_info[i+j*m_Lab.w].north=true;
                cell_info[i+j*m_Lab.w].visited=true;
                pushStack(stack,i,j);
                break;
            }
            visited++;
        }

        //Si no ha vecinos disponibles vuelve hacia la casilla anterior para ver si puede moverse hacia otro lado
        else
        {
            last=popStack(stack);
            i=stack[last].x;
            j=stack[last].y;
        }

    } while(visited<N); //No termina hasta que todas las celdas estén visitadas

    //Dibuja las celdas con sus respectivos muros
    for(j=0;j<m_Lab.h*2+1;j++)
    {
        for(i=0;i<m_Lab.w*2+1;i++)
        {
            if(i%2==0||j%2==0)
            {
                m_Lab.esq[i+j*(m_Lab.w*2+1)]='#';

            }
        }
    }

    //Quita los muros para dibujar el equema
    for(j=0;j<m_Lab.h*2+1;j++)
    {
        for(i=0;i<m_Lab.w*2+1;i++)
        {
            if((i%2!=0)&&(j%2!=0))
            {
                m_Lab.esq[i+j*(m_Lab.w*2+1)]=' ';
                lTOi= i-(i/2+1)+(j-(j/2+1))*m_Lab.w; //Relacion entre las celdas de información y nuestras celdas en el grid

                if(cell_info[lTOi].east==true)
                    m_Lab.esq[i+j*(m_Lab.w*2+1)+1]=' ';

                if(cell_info[lTOi].north==true)
                    m_Lab.esq[i+(j-1)*(m_Lab.w*2+1)]=' ';
            }
        }
    }

    free(cell_info);
    free(stack);
}

//Una funcion para dar muros y que se puedan dibujar ya en pantalla
void drawLab(Window window, M_Lab m_Lab, Entity muros[], Textures tex)
{
    int i,j,nmuro=0;
    int width=5;
    int large=235;


    //Indice:0->arriba, 1->izquierda, 2->abajo y 3->derecha
    for(j=0;nmuro<4;j++)
    {
        for(i=0;i<m_Lab.w*2+1;i++)
        {
                    //Los muros externos
                    if(((i==0)||(i==m_Lab.w*2))&&(j==0))
                    {
                        muros[nmuro].tex=tex.wall;
                        muros[nmuro].src.x=0;
                        muros[nmuro].src.y=0;
                        SDL_QueryTexture(tex.wall,NULL,NULL,&muros[nmuro].src.w,&muros[nmuro].src.h);
                        muros[nmuro].dst.h=(large)*m_Lab.h-width;
                        muros[nmuro].dst.w=width;
                        muros[nmuro].dst.x=(large-width)*(i-i/2);
                        muros[nmuro].dst.y=0;
                        nmuro++;
                    }

                    if(((j==0)||(j==m_Lab.h*2))&&(i==0))
                    {
                        muros[nmuro].tex=tex.wall;
                        muros[nmuro].src.x=0;
                        muros[nmuro].src.y=0;
                        SDL_QueryTexture(tex.wall,NULL,NULL,&muros[nmuro].src.w,&muros[nmuro].src.h);
                        muros[nmuro].dst.h=width;
                        muros[nmuro].dst.w=(large-width)*m_Lab.w;
                        muros[nmuro].dst.x=0;
                        muros[nmuro].dst.y=(large-width)*(j-j/2);
                        nmuro++;
                    }
                }
        }

    for(j=0;j<m_Lab.h*2+1;j++)
    {
        for(i=0;i<m_Lab.w*2+1;i++)
        {
            //Los que no son muros externos
            if((i!=0)&&(j!=0)&&(i!=m_Lab.w*2)&&(j!=m_Lab.h*2))
                {
                    //Muros verticales
                    if((j%2!=0)&&(m_Lab.esq[i+j*(2*m_Lab.w+1)]=='#'))
                    {
                        muros[nmuro].tex=tex.wall;
                        muros[nmuro].src.x=0;
                        muros[nmuro].src.y=0;
                        SDL_QueryTexture(tex.wall,NULL,NULL,&muros[nmuro].src.w,&muros[nmuro].src.h);
                        muros[nmuro].dst.h=large;
                        muros[nmuro].dst.w=width;
                        muros[nmuro].dst.x=(large-width)*(i-i/2);
                        muros[nmuro].dst.y=(large-width)*(j-j/2-1);
                        nmuro++;
                    }

                    //Muros horizontales
                    if((i%2!=0)&&(j%2==0)&&(m_Lab.esq[i+j*(2*m_Lab.w+1)]=='#'))
                    {
                        muros[nmuro].tex=tex.wall;
                        muros[nmuro].src.x=0;
                        muros[nmuro].src.y=0;
                        SDL_QueryTexture(tex.wall,NULL,NULL,&muros[nmuro].src.w,&muros[nmuro].src.h);
                        muros[nmuro].dst.h=width;
                        muros[nmuro].dst.w=large;
                        muros[nmuro].dst.x=(large-width)*(i-i/2-1);
                        muros[nmuro].dst.y=(large-width)*(j-j/2);
                        nmuro++;
                    }
                }
        }
    }

}


//Una funcion para desarrollador que permite ver el laberinto que se genera
void DebugLab(M_Lab m_Lab)
{
    int i, j;
    for(j=0;j<(2*m_Lab.h+1);j++)
    {
        for(i=0;i<(2*m_Lab.w+1);i++)
        {
            printf("%c",m_Lab.esq[i+j*(2*m_Lab.w+1)]);
        }
        printf("\n");
    }
}

void generarBots(M_Lab m_Lab,Bot bots[], int nbots, Textures tex, const Vector2i pPlayer, Vector2i desfase)
{
    int i,j, ncells, put;
    const int w=235;
    Cell *cell_info;
    Vector2i p[nbots];
    put=0;

    ncells=m_Lab.h*m_Lab.w;

    cell_info=malloc(ncells*sizeof(Cell));
    if(cell_info==NULL)
    {
        printf("Error reservando memoria");
        exit(-1);
    }

    //Comprueba las casillas donde no queremos que ponga bots
    ncells=0;

    for(i=0;i<m_Lab.w;i++)
    {
        for(j=0;j<m_Lab.h;j++)
        {
            if(sqrt(pow(i-pPlayer.x,2)+pow(j-pPlayer.y,2))>1)
            {
                cell_info[i+j*m_Lab.w].visited=false;
                ncells++;
            }
            else
                cell_info[i+j*m_Lab.w].visited=true;
        }
    }

    //Pone todos los bots, intentando que estén lo más separado posible
    while(put<nbots)
    {
        i=rand() % m_Lab.w;
        j=rand() % m_Lab.h;

        if(cell_info[i+j*m_Lab.w].visited==false)
        {
            p[put].x=i*w+w/2-40/2-desfase.x;
            p[put].y=j*w+w/2-70/2-desfase.y;
            cell_info[i+j*m_Lab.w].visited=true;
            put++;
            ncells--;

        }
        else if(ncells==0)
        {
            for(i=0;i<m_Lab.w;i++)
            {
                for(j=0;j<m_Lab.h;j++)
                {
                    if(sqrt(pow(i-pPlayer.x,2)+pow(j-pPlayer.y,2))>1)
                    {
                        cell_info[i+j*m_Lab.w].visited=false;
                        ncells++;
                    }
                    else
                        cell_info[i+j*m_Lab.w].visited=true;
                }
            }
        }

    }

    free(cell_info);

    bot_creator(bots,p,tex.bot,nbots);
}

void generarTokens(M_Lab m_Lab,Tokens tokens[], int ncafe, int ndine, int ncharcos, Textures tex, const Vector2i pPlayer, const Vector2i pSalida, Vector2i desfase)
{
    int i,j,k, put, ncells ,ntoken;
    Cell *cell_info;
    const int w=235;
    Vector2i p[(ncafe+ndine+ncharcos)];
    put=0;

    ncells=m_Lab.h*m_Lab.w;

    cell_info=malloc(ncells*sizeof(Cell));
    if(cell_info==NULL)
    {
        printf("Error reservando memoria");
        exit(-1);
    }

    for(k=0;k<3;k++)
    {

    switch(k)
    {
    case 0:
        ntoken=ncafe;
        break;

    case 1:
        ntoken+=ndine;
        break;

    case 2:
        ntoken+=ncharcos;
        break;
    }

    //Comprueba las casillas que no nos sirven
    ncells=0;

    for(i=0;i<m_Lab.w;i++)
    {
        for(j=0;j<m_Lab.h;j++)
        {
            if(((i!=pPlayer.x)||(j!=pPlayer.y))&&((i!=pSalida.x)||(j!=pSalida.y)))
            {
                cell_info[i+j*m_Lab.w].visited=false;
                ncells++;
            }
            else
                cell_info[i+j*m_Lab.w].visited=true;
        }
    }


    //Pone todos los tokens, intentando que estén lo más separado posible
    while(put<ntoken)
    {
        i=rand() % m_Lab.w;
        j=rand() % m_Lab.h;

        if(cell_info[i+j*m_Lab.w].visited==false)
        {
            p[put].x=i*w-desfase.x;
            p[put].y=j*w-desfase.y;
            cell_info[i+j*m_Lab.w].visited=true;
            put++;
            ncells--;

        }
        else if(ncells==0)
        {
            for(i=0;i<m_Lab.w;i++)
            {
                for(j=0;j<m_Lab.h;j++)
                {
                    if(((i!=pPlayer.x)||(j!=pPlayer.y))&&((i!=pSalida.x)||(j!=pSalida.y)))
                    {
                        cell_info[i+j*m_Lab.w].visited=false;
                        ncells++;
                    }
                    else
                        cell_info[i+j*m_Lab.w].visited=true;
                }
            }
        }

    }
    }

    free(cell_info);

    TokensCreator(tokens, tex, p, ncafe, ndine, ncharcos);
}

void generarSalida(Entity *salida, M_Lab m_Lab, const Vector2i pPlayer, Textures tex, Vector2i *pSalida, Vector2i desfase)
{
    const int large=235;
    const int width=5;
    int r[4],rmax;
    int i,j,put;
    r[0]=abs(pPlayer.x);
    r[1]=abs(m_Lab.w-(pPlayer.x+1));
    r[2]=abs(pPlayer.y);
    r[3]=abs(m_Lab.w-(pPlayer.x+1));

    rmax=max(r,4);

    put=0;

    while(put==0)
    {
        i=rand()%m_Lab.w;
        j=rand()%m_Lab.h;
        if(sqrt(pow(i-pPlayer.x,2)+pow(j-pPlayer.y,2))>rmax-1)
        {
            pSalida->x=i;
            pSalida->y=j;
            salida->dst.x=(large-width)*i+110-desfase.x;
            salida->dst.y=(large-width)*j+45-desfase.y;
            put++;
        }
    }

    salida->tex=tex.salida;
    salida->src.x=0;
    salida->src.y=0;
    SDL_QueryTexture(tex.salida,NULL,NULL,&salida->src.w,&salida->src.h);
    salida->dst.h=150;
    salida->dst.w=120;

}

void desfasarMuros(Entity muros[],int nmuros, Vector2i desfase)
{
    int i;

    for(i=0;i<nmuros;i++)
    {
        muros[i].dst.x-=desfase.x;
        muros[i].dst.y-=desfase.y;
    }
}

void renderLab(Window window, Entity muros[], int nmuros, Entity salida)
{
    int i;
    for(i=0;i<nmuros;i++)
            SDL_RenderCopy(window.renderer, muros[i].tex, &muros[i].src, &muros[i].dst);

    SDL_RenderCopy(window.renderer, salida.tex, &salida.src, &salida.dst);
}

void movement(Window win, Entity muros[], Entity *salida, int nmuros, key_buttons k, player_t *player, Bot bots[],Tokens Tok[], int ntokens, int nbots, bool boton, float delta_time, int velocidad)
{
    float velocity;
    int inc,ghx,ghy,aux,ww=5;
    int WIDTH=40,HEIGTH=70;
    Vector2f v;
    v.x = player->texture.w;
    v.y = player->texture.h;

    if((boton == false)||(velocidad == 0)) //Velocidad se reduce mientras el botón no está cargado o al pisar el charco
        velocity = 175;
    else if(velocidad == 2)
        velocity = 500;
    else
        velocity = 300;
    inc=(int)(velocity*delta_time);

    if(k.W==true)
    {
        ghy=player->texture.y+HEIGTH/2-inc;
        if(ComprobarMuros(player->texture.x,ghy,v,muros,nmuros)!=1)
        {
            if((muros[0].dst.y>-2)&&(ghy<win.h/2+2))
            {
                aux=-muros[0].dst.y;
                movPlayer(inc-aux,player,0);
                movLab(aux,muros,nmuros,salida,bots,nbots,Tok,ntokens,0);
                printf("Hola");
            }
            else
            {
                aux=win.w/2-HEIGTH/2-player->texture.y;
                movPlayer(aux,player,0);
                movLab(inc-aux,muros,nmuros,salida,bots,nbots,Tok,ntokens,0);
            }
        }

    }
    if(k.S==true)
    {
        ghy=player->texture.y+HEIGTH/2+inc;
        if(ComprobarMuros(player->texture.x,ghy,v,muros,nmuros)!=1)
        {
            if((muros[2].dst.y<win.h+2)&&(ghy>win.h/2-2))
            {
                aux=win.h-muros[2].dst.y;
                movPlayer(inc-aux,player,1);
                movLab(aux,muros,nmuros,salida,bots,nbots,Tok,ntokens,1);
            }
            else
            {
                aux=-win.h/2+HEIGTH/2+player->texture.y;
                movPlayer(aux,player,1);
                movLab(inc-aux,muros,nmuros,salida,bots,nbots,Tok,ntokens,1);
            }
        }

    }/*
    if(k.A==true)
    {
        ghx=player->texture.x+WIDTH/2-inc;
        if(ComprobarMuros(ghx,player->texture.y,v,muros,nmuros)!=1)
        {
            if((muros[1].dst.x>-2)&&(ghx<win.w/2+2))
            {
                aux=-muros[1].dst.x;
                movPlayer(inc-aux,player,2);
                movLab(aux,muros,nmuros,salida,bots,nbots,Tok,ntokens,2);
            }
            else
            {
                aux=win.w/2-WIDTH/2-player->texture.y;
                movPlayer(aux,player,2);
                movLab(inc-aux,muros,nmuros,salida,bots,nbots,Tok,ntokens,2);
            }
        }

    }
    if(k.D==true)
    {
        ghx=player->texture.x+WIDTH/2+inc;
        if(ComprobarMuros(ghx,player->texture.y,v,muros,nmuros)!=1)
        {
            if((muros[3].dst.x<win.w+2)&&(ghx>win.w/2-2))
            {
                aux=-win.w/2+WIDTH/2+player->texture.x;
                movPlayer(inc-aux,player,3);
                movLab(aux,muros,nmuros,salida,bots,nbots,Tok,ntokens,3);
            }
            else
            {
                aux=win.w/2-WIDTH/2-player->texture.y;
                movPlayer(aux,player,3);
                movLab(inc-aux,muros,nmuros,salida,bots,nbots,Tok,ntokens,3);
            }
        }

}*/
}

//Mueve el laberinto dando la sensación de que se mueve el personaje
void movLab(int inc,Entity muros[],int nmuros,Entity *salida,Bot bots[],int nbots,Tokens Tok[],int ntokens,int d)
{
    int i;

    if(d==0)
    {

            for(i=0; i<ntokens; i++)
            Tok[i].entity.dst.y+=inc;

            for(i=0;i<nbots;i++)
            bots[i].entity.dst.y+=inc;

            salida->dst.y+=inc;
    }
    if(d==2)
    {
            for(i=0; i<ntokens; i++)
            Tok[i].entity.dst.x+=inc;

            for(i=0;i<nbots;i++)
            bots[i].entity.dst.x+=inc;

            salida->dst.x+=inc;
    }
    if(d==3)
    {
            for(i=0; i<ntokens; i++)
            Tok[i].entity.dst.x-=inc;

            for(i=0;i<nbots;i++)
            bots[i].entity.dst.x-=inc;

            salida->dst.x-=inc;
    }
    if(d==1)
    {
            for(i=0; i<ntokens; i++)
            Tok[i].entity.dst.y-=inc;

            for(i=0;i<nbots;i++)
            bots[i].entity.dst.y-=inc;

            salida->dst.y-=inc;
    }
}

void movPlayer(int inc,player_t *player,int d)
{
    if(d==0)
            player->texture.y+=inc;

    if(d==2)
            player->texture.x+=inc;

    if(d==3)
            player->texture.x-=inc;

    if(d==1)
            player->texture.y-=inc;
}

