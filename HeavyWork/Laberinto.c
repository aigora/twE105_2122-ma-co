#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "Laberinto.h"
#include "Utilities.h"
#include "Enemies.h"

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

    //Genera una semilla de aleatoriedad mirando al tiempo
    srand(time(NULL));

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
        //printf("%i %i \n",j,i);

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
void drawLab(Window window, M_Lab m_Lab, Entity muros[], Entity *salida, Textures tex)
{
    int i,j,nmuro=0;
    int width=5;
    int large=235;

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
                        muros[nmuro].dst.h= large;
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

                //Los muros externos
                else
                {
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
    }

    salida->tex=tex.salida;
    salida->src.x=0;
    salida->src.y=0;
    SDL_QueryTexture(tex.salida,NULL,NULL,&salida->src.w,&salida->src.h);
    salida->dst.h=150;
    salida->dst.w=200;
    salida->dst.x=(large-width)*(m_Lab.w-1)+30;
    salida->dst.y=(large-width)*(m_Lab.h-1)+45;

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

//Mueve el laberinto dando la sensación de que se mueve el personaje
void movLab(Entity muros[], Entity *salida, int nmuros, key_buttons k, player_t player, Bot bots[],Entity Tok[], int ntokens, int nbots, bool boton, float delta_time, int velocidad)
{
    int i, j;
    float velocity;
    int position;
    if((boton == false)||(velocidad == 0)) //Velocidad se reduce mientras el botón no está cargado o al pisar el charco
        velocity = 175;
    else if(velocidad == 2)
        velocity = 500;
    else
        velocity = 300;

    Vector2f v;
    v.x = player.texture.w;
    v.y = player.texture.h;
    position=(int)(velocity*delta_time);

    if(k.W==true)
    {
        for(i=0; i<nmuros; i++)
            muros[i].dst.y+=position;

        if(ComprobarMuros(player.texture.x, player.texture.y, v, muros, nmuros)==1)
                for(j=0; j<nmuros; j++)
                    muros[j].dst.y-=position;
        else
        {
            for(i=0; i<ntokens; i++)
            Tok[i].dst.y+=position;

            for(i=0;i<nbots;i++)
            bots[i].entity.dst.y+=position;

            salida->dst.y+=position;
        }
    }
    if(k.A==true)
    {
        for(i=0; i<nmuros; i++)
            muros[i].dst.x+=position;

        if(ComprobarMuros(player.texture.x, player.texture.y, v, muros, nmuros)==1)
                for(j=0; j<nmuros; j++)
                    muros[j].dst.x-=position;
        else
        {
            for(i=0; i<ntokens; i++)
            Tok[i].dst.x+=position;

            for(i=0;i<nbots;i++)
            bots[i].entity.dst.x+=position;

            salida->dst.x+=position;
        }
    }
    if(k.D==true)
    {
        for(i=0; i<nmuros; i++)
            muros[i].dst.x-=position;

        if(ComprobarMuros(player.texture.x, player.texture.y, v, muros, nmuros)==1)
                for(j=0; j<nmuros; j++)
                    muros[j].dst.x+=position;
        else
        {
            for(i=0; i<ntokens; i++)
            Tok[i].dst.x-=position;

            for(i=0;i<nbots;i++)
            bots[i].entity.dst.x-=position;

            salida->dst.x-=position;
        }
    }
    if(k.S==true)
    {
        for(i=0; i<nmuros; i++)
            muros[i].dst.y-=position;

        if(ComprobarMuros(player.texture.x, player.texture.y, v, muros, nmuros)==1)
                for(j=0; j<nmuros; j++)
                    muros[j].dst.y+=position;
        else
        {
            for(i=0; i<ntokens; i++)
            Tok[i].dst.y-=position;

            for(i=0;i<nbots;i++)
            bots[i].entity.dst.y-=position;

            salida->dst.y-=position;
        }
    }
}

void generarBots(M_Lab m_Lab,Bot bots[], int nbots, Textures tex)
{
    int i,j, nx=0, ny=0, put, x[m_Lab.w],y[m_Lab.h];
    const int w=235;
    Vector2i p[nbots];
    put=0;
    const Vector2i pPlayer={0,0};

    for(i=0;i<m_Lab.w;i++)
        if(i!=pPlayer.x)
        {
        x[i]=i;
        nx++;
        }
        else
            x[i]=NULL;

    for(i=0;i<m_Lab.h;i++)
        if(i!=pPlayer.y)
        {
        y[i]=i;
        ny++;
        }
        else
            y[i]=NULL;

    srand(time(NULL));

    while(put<nbots)
    {
        i=rand() % m_Lab.w;
        j=rand() % m_Lab.h;

        if((x[i]!=NULL)&&(y[j]!=NULL))
        {
            p[put].x=i*235+117;
            p[put].y=j*235+117;
            put++;
            x[i]=NULL;
            nx--;
            y[i]=NULL;
            ny--;
        }
        else if((nx==0)||(ny==0))
        {
            for(i=0;i<m_Lab.w;i++)
              if(i!=pPlayer.x)
              {
                x[i]=i;
                nx++;
              }
              else
                x[i]=NULL;

            for(i=0;i<m_Lab.h;i++)
                if(i!=pPlayer.y)
                {
                  y[i]=i;
                  ny++;
                }
                else
                    y[i]=NULL;
        }

    }

    bot_creator(bots,p,tex.bot,nbots);
}

void renderLab(Window window, Entity muros[], int nmuros, Entity salida)
{
    int i;
    for(i=0;i<nmuros;i++)
            SDL_RenderCopy(window.renderer, muros[i].tex, &muros[i].src, &muros[i].dst);

    SDL_RenderCopy(window.renderer, salida.tex, &salida.src, &salida.dst);
}
