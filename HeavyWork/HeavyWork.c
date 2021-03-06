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
#include "Font.h"

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

    bool success = true;
    if (TTF_Init() == -1) {
        printf("Error al inicializar SDL_TTF error = %s\n", TTF_GetError());
        success = false;
    }

	//Cargamos las texturas que vayamos a usar
    tex.playerinv = loadTexture("resources/playerdefinv.png",mainWin);
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
	tex.ayuda = loadTexture("Resources/Ayuda.jpg",mainWin);

	tex.wall = loadTexture("resources/Negro.jpg",mainWin);
    tex.fondo = loadTexture("resources/gris.jpg",mainWin);
    tex.prox = loadTexture("resources/Proximamente.jpg",mainWin);
    tex.creditos = loadTexture("resources/creditos.jpg",mainWin);

    tex.vida = loadTexture("resources/life.png",mainWin);
    tex.pause = loadTexture("resources/pause.jpg",mainWin);
    tex.billete = loadTexture("resources/Pbillete.png",mainWin);
    tex.cafe = loadTexture("resources/Taza.png",mainWin);
    tex.charco = loadTexture("resources/charco.png",mainWin);
    tex.salida = loadTexture("resources/Escaleras.png",mainWin);
    tex.boton = loadTexture("resources/aceptar.png",mainWin);
    tex.escaleras = loadTexture("resources/carganivel.jpg",mainWin);

    SDL_Color colour = { .r=255, .g=255, .b=255, .a=255 };
    font_texture_t titulo = newText("Nombre de la partida:", colour, 24, mainWin);
    tex.titulo_puntuacion = titulo.texture;

    //La m???sica que se reproducir???
    Mix_Music *musica = NULL;

    //Efectos de sonido que se usar???n
    Mix_Chunk *recoger = NULL;
    Mix_Chunk *invisi = NULL;

    //Inicializar SDL_mixer
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    //Cargar m???sica
    musica = Mix_LoadMUS( "resources/music2.wav" );
    if( musica == NULL )
    {
        printf( "Fallo al cargar la m???sica, SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    //Cargar efectos de sonido
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

    long long int score = 0;
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
            stage=game(mainWin,tex, recoger, invisi, &score);
            break;
        case 3:
            // Pantalla de salida
            exitScreen(mainWin, tex, score);
            stage = 1;
            break;
        }
    }

    SDL_DestroyRenderer(mainWin.renderer);
    SDL_DestroyWindow(mainWin.window);

    //Liberar efectos de sonido
    Mix_FreeChunk( recoger );

    //Liberar m???sica
    Mix_FreeMusic( musica );

    SDL_Quit();
    return 0;
}
