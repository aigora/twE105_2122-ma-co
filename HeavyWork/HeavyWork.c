#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_audio.h>
#include "Game.h"
#include "Menu.h"
#include "Player.h"

//velocidad en pixeles por segundo
#define SPEED 300

#define MAX_VIDAS   3

int main(int argc, char *argv[])
{
    int p;
    bool running=true;
    int stage = 1;  //Indica en que fase del flowchart estamos
    int sonido = 0; //Indica si el sonido se encuentra habilitado o no, inicialmente encendido
    int personaje = 1; //Indica el personaje seleccionado, por defecto el stickman
    Window mainWin;
    Textures tex;



    // Se inicializa SDL, con todos los subsistemas y se comprueba si da error
    if(SDL_Init(SDL_INIT_EVERYTHING)!=0)
    {
        printf("Error al iniciar SDL: %s\n", SDL_GetError());
        return 1;
    }
	mainWin.h=650;
	mainWin.w=1000;

	Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    mainWin.window = SDL_CreateWindow("HeavyWork",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,mainWin.w,mainWin.h,0);
    if(!mainWin.window)
    {
        printf("Error al crear la ventana: %s\n", SDL_GetError());
        running=false;
    }

    mainWin.renderer = SDL_CreateRenderer(mainWin.window,-1,render_flags);
    if(!mainWin.renderer)
    {
        printf("No funciona");
        printf("Error al crear el renderer: %s\n", SDL_GetError());
        running=false;
    }
	//running=loadWin("HeavyWork",mainWin);

	//Cargamos las texturas que vayamos a usar
	tex.player=loadTexture("Resources/playerdef.png",mainWin);
    tex.bot=loadTexture("Resources/player_gri2def.png",mainWin);
    tex.menu=loadTexture("Resources/Menuinicio2.jpg",mainWin);
	tex.ajustes=loadTexture("Resources/ajustes.jpg",mainWin);
	tex.ajusnos=loadTexture("Resources/ajustessinson.jpg",mainWin);
	tex.carga=loadTexture("Resources/carga.jpg",mainWin);
	tex.personaje1=loadTexture("Resources/personaje1.jpg",mainWin);
	tex.personaje2=loadTexture("Resources/personaje2.jpg",mainWin);
	tex.personaje3=loadTexture("Resources/personaje3.jpg",mainWin);
	tex.wall = loadTexture("resources/Negro.jpg",mainWin);
    tex.fondo = loadTexture("resources/parquetg.jpg",mainWin);
    tex.prox = loadTexture("resources/Proximamente.jpg",mainWin);
    tex.png = loadTexture("resources/PruebaPNG.png",mainWin);
    tex.vida = loadTexture("resources/life.png",mainWin);

    // Instanciar jugador
    Vector2i pos = { 50, 100 };
    player_t* player = newPlayer(pos, MAX_VIDAS, tex.vida, tex.player);
    //Iniciar un bot
    Vector2i coordbot = {500,300};
    bot_struct* bot = bot_creator(coordbot,tex.bot);

	while(running)
    {
        switch(stage)
        {
        case 0:
            running=false;
            break;
        case 1:
            stage=menu(mainWin,tex,&personaje, sonido);
            break;
        case 2:
            stage=game(mainWin,tex,player,bot);
            break;
        }
    }

    SDL_DestroyRenderer(mainWin.renderer);
    SDL_DestroyWindow(mainWin.window);
    SDL_Quit();
    return 0;

}
