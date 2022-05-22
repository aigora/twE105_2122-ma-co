#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_audio.h>
#include "Game.h"
#include "Menu.h"
#include "Player.h"
#include "Tokens.h"

#define SPEED 300    //velocidad en pixeles por segundo
#define MAX_VIDAS   3

int main(int argc, char *argv[])
{

    bool running=true, sonido = true;//Indica si el sonido se encuentra habilitado o no
    int stage = 1;  //Indica en que fase del flowchart estamos
    int personaje = 1; //Indica el personaje seleccionado, por defecto el stickman
    Window mainWin;
    Textures tex;
    mainWin.h=650;
	mainWin.w=1000;


    // Se inicializa SDL, con todos los subsistemas y se comprueba si da error
    if(SDL_Init(SDL_INIT_EVERYTHING)!=0)
    {
        printf("Error al iniciar SDL: %s\n", SDL_GetError());
        return 1;
    }

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

	//Cargamos las texturas que vayamos a usar
    tex.playerinv = loadTexture("resources/persinv.png",mainWin);
    tex.playerinvdrcha = loadTexture("resources/playerdefinvdrcha.png",mainWin);
    tex.playerinvizqda = loadTexture("resources/playerdefinvizqda.png",mainWin);
    tex.playerinvatras = loadTexture("resources/playerdefinvatras.png",mainWin);

	tex.player = loadTexture("Resources/playerdef.png",mainWin);
    tex.playerdrcha = loadTexture("resources/playerdefdrcha.png",mainWin);
    tex.playerizqda = loadTexture("resources/playerdefizqda.png",mainWin);
    tex.playeratras = loadTexture("resources/playerdefatras.png",mainWin);

    tex.bot = loadTexture("Resources/player_gri2def.png",mainWin);

    tex.menu = loadTexture("Resources/menu2.jpg",mainWin);
	tex.ajustes = loadTexture("Resources/ajustes2.jpg",mainWin);
	tex.ajusnos = loadTexture("Resources/ajustesno2.jpg",mainWin);
	tex.carga = loadTexture("Resources/carga2.jpg",mainWin);
	tex.personaje1 = loadTexture("Resources/personajesn.jpg",mainWin);
	tex.personaje2 = loadTexture("Resources/personaje2.jpg",mainWin);
	tex.personaje3 = loadTexture("Resources/personaje3.jpg",mainWin);

	tex.wall = loadTexture("resources/Negro.jpg",mainWin);
    tex.fondo = loadTexture("resources/parquetg.jpg",mainWin);
    tex.prox = loadTexture("resources/Proximamente.jpg",mainWin);
    tex.creditos = loadTexture("resources/creditos.jpg",mainWin);

    tex.png = loadTexture("resources/PruebaPNG.png",mainWin);
    tex.vida = loadTexture("resources/life.png",mainWin);
    tex.vision = loadTexture("resources/Vision.png",mainWin);
    tex.pause = loadTexture("resources/pause.jpg",mainWin);
    tex.billete = loadTexture("resources/Pbillete.png",mainWin);
    tex.cafe = loadTexture("resources/Taza.png",mainWin);
    tex.charco = loadTexture("resources/charco.png",mainWin);
    tex.salida = loadTexture("resources/Escaleras.png",mainWin);


    // Instanciar jugador
    Vector2i pos = { 250, 300 };
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
    player_t* player = newPlayer(pos, MAX_VIDAS, player_textures);

    //La m�sica que se reproducir�
    Mix_Music *musica = NULL;

    //Efectos de sonido que se usar�n
    Mix_Chunk *recoger = NULL;
    Mix_Chunk *invisi = NULL;

    bool success = true;

    //Inicializar SDL_mixer
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    //Cargar m�sica
    musica = Mix_LoadMUS( "resources/music2.wav" );
    if( musica == NULL )
    {
        printf( "Fallo al cargar la m�sica, SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    //Cargar efectoss de sonido
    recoger = Mix_LoadWAV( "resources/recoger2.wav" );
    if( recoger == NULL )
    {
        printf( "Fallo al cargar el efecto de recoger, SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    invisi = Mix_LoadWAV( "resources/invisi.wav" );//Efecto de sonido para el superpoder invisibilidad
    if( invisi == NULL )
    {
        printf( "Fallo al cargar el efecto de la invisibilidad, SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

	while(running)
    {
        switch(stage)
        {
        case 0:
            running=false;
            break;
        case 1:
            stage=menu(mainWin,tex,&personaje, sonido, musica);
            break;
        case 2:
            stage=game(mainWin,tex,player, recoger, invisi);
            break;
        }
    }

    SDL_DestroyRenderer(mainWin.renderer);
    SDL_DestroyWindow(mainWin.window);

    //Liberar efectos de sonido
    Mix_FreeChunk( recoger );

    //Liberar m�sica
    Mix_FreeMusic( musica );

    SDL_Quit();
    return 0;
}
