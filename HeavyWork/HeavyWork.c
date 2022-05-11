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
/*
    // Para cargar un archivo de audio, el formato reconocido por la librer�a b�sica de SDL es WAV. El clip de audio es cargado:
    SDL_AudioSpec wavSpec;
	Uint32 wavLength;
	Uint8 *wavBuffer;

	SDL_LoadWAV("resources/music2.wav", &wavSpec, &wavBuffer, &wavLength);

	// Con el siguiente comando se abre el dispositivo de audio que lo va a reproducir

	SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);

	// Los argumentos del comando anterior hacen referencia, el primero al nombre del dispositivo que lo reproducir� (al pasarle NULL
    // tomar� el dispositivo predeterminado, el segundo es relevante en relaci�n a dispositivos de grabaci�n, no de reproducci�n, el
    // tercero representa el formato del audio de entrada, el cuarto el formato del audio de salida y el quinto se refiere a escenarios avanzados de audio

	// Reproducir la pista

	int success = SDL_QueueAudio(deviceId, wavBuffer, wavLength); // SDL_QueueAudio permite enviar la informaci�n del WAV directamente al dispositivo
*/
	//Cargamos la ventana del juego
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
	tex.player=loadTexture("Resources/player_mar.png",mainWin);
    tex.bot=loadTexture("Resources/player_gri2.png",mainWin);
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

    /*SDL_Event event; //Creamos una variable de tipo evento
    int SDL_CaptureMouse(SDL_bool enabled);//Relativo a la detecci�n del rat�n


    SDL_Rect dest;

    //Consigue y escaliza las dimensiones de la textura
    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
    dest.w/= 4;
    dest.h /= 4;

    //Se inicia en el centro de la pantalla
    float x_pos = (1000 - dest.w)/2;
    float y_pos = (650 - dest.h)/2;
    float x_vel = 0;
    float y_vel = 0;

    //Recoge el proceso que se est� realizando
    int up = 0;
    int down = 0;
    int left = 0;
    int right = 0;

    //Establece 1 cuando se presiona el bott�n de cerrado
    int close_requested = 0;





 while(stage!=1)
    {
        if(sonido==1)
        {
        SDL_PauseAudioDevice(deviceId, 0); //Pausa la grabaci�n de audio al darle un n�mero != 0
        }
        if(sonido==0)
        {
        SDL_PauseAudioDevice(deviceId, 1);
        }

        //Bucle del juego
        while(stage==0)
        {
            SDL_Surface* surface = IMG_Load("resources/Menuinicio2.jpg"); //Carga una superficie con la imagen de prueba en la memoria

            if(!surface)
            {
                printf("Error creando surface: %s\n", SDL_GetError());
                SDL_DestroyRenderer(rend);
                SDL_DestroyWindow(mainWin);
                SDL_Quit();
                return 0;
            }

            SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, surface);
            SDL_FreeSurface(surface);
            if(!texture)
            {
                printf("Error creando texture: %s\n", SDL_GetError());
                SDL_DestroyRenderer(rend);
                SDL_DestroyWindow(mainWin);
                SDL_Quit();
                return 0;
            }
            int mouse_x, mouse_y;
            while(stage==0){
                int buttons = SDL_GetMouseState(&mouse_x, &mouse_y); //Adjunta unas coordenadas al mouse

                SDL_RenderClear(rend); //Limpia la ventana

                //Dibuja la imagen
                SDL_RenderCopy(rend, texture, NULL, NULL);
                SDL_RenderPresent(rend);

                while(SDL_PollEvent(&event))
                {
                    if(event.type==SDL_QUIT) //Permite salir de la ventana si se cierra arriba a la derecha
                        {
                            stage=1;
                            SDL_DestroyTexture(texture);
                        }

                    if(mouse_x < 450 && mouse_y > 420 && mouse_x > 120 && mouse_y < 500)
                    {
                        if(event.type==SDL_MOUSEBUTTONUP) //JUGAR
                        {
                            stage=2;
                            SDL_DestroyTexture(texture);
                        }
                    }
                    if(mouse_x > 550 && mouse_y > 420 && mouse_x < 880 && mouse_y < 500) //Selecciona en que parte de la pantalla puedo clickar
                    {
                        if(event.type==SDL_MOUSEBUTTONUP) //SCORES
                        {
                            stage=3;
                            SDL_DestroyTexture(texture);
                        }
                    }
                    if ( mouse_x > 120 && mouse_x < 450 && mouse_y > 520 && mouse_y < 600)
                    {
                        if(event.type==SDL_MOUSEBUTTONUP) //AJUSTES
                        {
                            if (sonido == 1)
                            {
                            stage=4;
                            SDL_DestroyTexture(texture);
                            }
                            if (sonido == 0)
                            {
                            stage=5;
                            SDL_DestroyTexture(texture);
                            }
                        }
                    }
                    if (mouse_x > 550 && mouse_x < 880 && mouse_y > 520 && mouse_y < 600)
                    {
                         if(event.type==SDL_MOUSEBUTTONUP) //SALIR
                        {
                            stage=1;
                            SDL_DestroyTexture(texture);
                        }
                    }
                }
            }
    }
    int mouse_x, mouse_y, buttons;
    switch (stage)
    {
        case 2://JUGAR
                while(stage==2){
                    SDL_Surface* surface = IMG_Load("resources/carga.jpg"); //GENERA UNA NUEVA IMAGEN
                    SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, surface); //A�ADE LA NUEVA IMAGEN
                    SDL_FreeSurface(surface);
                    SDL_RenderCopy(rend, texture, NULL, NULL);//Genera un efecto de carga
                    SDL_RenderPresent(rend);
                    SDL_Delay(2000);
                    SDL_DestroyTexture(texture);


                    while(stage==2){

                        buttons = SDL_GetMouseState(&mouse_x, &mouse_y); //Adjunta unas coordenadas al mouse



                        while(SDL_PollEvent(&event))
                            {
                                if(event.type==SDL_QUIT) //Permite salir de la ventana si se cierra arriba a la derecha
                                {
                                    stage=1;
                                    SDL_DestroyTexture(texture);
                                }

                                //bucle de animaci�n
                                    switch(event.type) //Escanea los eventos
                                    {

                                    case SDL_KEYDOWN:
                                        switch (event.key.keysym.scancode)
                                        {
                                        case SDL_SCANCODE_W: //Movimiento hacia arriba
                                        case SDL_SCANCODE_UP:
                                            up = 1;
                                            break;
                                        case SDL_SCANCODE_A: //Movimiento hacia la izquierda
                                        case SDL_SCANCODE_LEFT:
                                            left = 1;
                                            break;
                                        case SDL_SCANCODE_S: ////Movimiento hacia abajo
                                        case SDL_SCANCODE_DOWN:
                                            down = 1;
                                            break;
                                        case SDL_SCANCODE_D: //Movimiento hacia la derecha
                                        case SDL_SCANCODE_RIGHT:
                                            right = 1;
                                            break;
                                        }
                                        break;
                                        case SDL_KEYUP:
                                            switch (event.key.keysym.scancode)
                                            {
                                        case SDL_SCANCODE_W:
                                        case SDL_SCANCODE_UP:
                                            up = 0;
                                            break;
                                        case SDL_SCANCODE_A:
                                        case SDL_SCANCODE_LEFT:
                                            left = 0;
                                            break;
                                        case SDL_SCANCODE_S:
                                        case SDL_SCANCODE_DOWN:
                                            down = 0;
                                            break;
                                        case SDL_SCANCODE_D:
                                        case SDL_SCANCODE_RIGHT:
                                            right = 0;
                                            break;
                                            }
                                            break;
                                    }
                                }

                                //Despeja la ventana
                                SDL_RenderClear(rend);

                                //Dibuja la imagen en la ventana
                                SDL_RenderCopy(rend, tex, NULL, &dest);
                                SDL_RenderPresent(rend);

                                //determina la velocidad
                                x_vel = y_vel = 0;
                                if(up && !down) y_vel = -SPEED;
                                if(down && !up) y_vel = SPEED;
                                if(left && !right) x_vel = -SPEED;
                                if (right && !left) x_vel = SPEED;

                                //actualizaci�n de posiciones
                                x_pos += x_vel / 60;
                                y_pos += y_vel / 60;

                                //detenci�n al impactar con los bordes
                                if(x_pos <= 0) x_pos = 0;
                                if(y_pos <= 0) y_pos = 0;
                                if(x_pos>=1000 - dest.w) x_pos = 1000 - dest.w;
                                if(y_pos>=650 - dest.h) y_pos = 650 - dest.h;

                                //Establece las posiciones de la estructura
                                dest.y = (int) y_pos;
                                dest.x = (int) x_pos;

                                //Espera 1/60 de un segundo
                                SDL_Delay(1000/60);
                                }
                            }

                            //Se inicia en el centro de la pantalla

            break;
        case 3://SCORES
                while(stage==3){
                    SDL_Surface*surface = IMG_Load("resources/Proximamente.jpg");
                    SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, surface); //A�ADE LA NUEVA IMAGEN
                    SDL_FreeSurface(surface);

                     while(stage==3){
                        buttons = SDL_GetMouseState(&mouse_x, &mouse_y); //Adjunta unas coordenadas al mouse

                        //Dibuja la imagen
                        SDL_RenderCopy(rend, texture, NULL, NULL);
                        SDL_RenderPresent(rend);

                        while(SDL_PollEvent(&event))
                            {
                                if(event.type==SDL_QUIT) //Permite salir de la ventana si se cierra arriba a la derecha
                                {
                                    stage=1;
                                    SDL_DestroyTexture(texture);
                                }
                            }
                    }
                }

            break;
        case 4://AJUSTES
                while(stage==4){
                    SDL_Surface*surface = IMG_Load("resources/ajustes.jpg");
                    SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, surface); //A�ADE LA NUEVA IMAGEN
                    SDL_FreeSurface(surface);

                    while(stage==4){
                        buttons = SDL_GetMouseState(&mouse_x, &mouse_y); //Adjunta unas coordenadas al mouse

                        //Dibuja la imagen
                        SDL_RenderCopy(rend, texture, NULL, NULL);
                        SDL_RenderPresent(rend);

                        while(SDL_PollEvent(&event))
                            {
                                if(event.type==SDL_QUIT) //Permite salir de la ventana con la cruceta
                                    {
                                        stage=1;
                                        SDL_DestroyTexture(texture);
                                    }

                                     if(mouse_x < 290 && mouse_y > 450 && mouse_x > 60 && mouse_y < 540)
                                {
                                    if(event.type==SDL_MOUSEBUTTONUP) //SONIDO
                                    {
                                        stage=5;
                                        sonido=0;
                                        SDL_DestroyTexture(texture);
                                    }
                                }
                                if(mouse_x > 400 && mouse_y > 450 && mouse_x < 630 && mouse_y < 540) //Selecciona en que parte de la pantalla puedo clickar
                                {
                                    if(event.type==SDL_MOUSEBUTTONUP) //PERSONAJE
                                    {
                                        stage=6;
                                        SDL_DestroyTexture(texture);
                                    }
                                }
                                if ( mouse_x > 710 && mouse_x < 900 && mouse_y > 450 && mouse_y < 540)
                                {
                                    if(event.type==SDL_MOUSEBUTTONUP) //USUARIO
                                    {
                                        stage=7;
                                        SDL_DestroyTexture(texture);
                                    }
                                }
                                if ( mouse_x > 830 && mouse_x < 980 && mouse_y > 575 && mouse_y < 600)
                                {
                                    if(event.type==SDL_MOUSEBUTTONUP) //VOLVER
                                    {
                                        stage=0;
                                        SDL_DestroyTexture(texture);
                                    }
                                }
                            }
                        }
                }
            break;
        case 5://SONIDO

                while(stage==5){
                    SDL_Surface*surface = IMG_Load("resources/ajustessinson.jpg");
                    SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, surface); //A�ADE LA NUEVA IMAGEN
                    SDL_FreeSurface(surface);

                    while(stage==5){
                        buttons = SDL_GetMouseState(&mouse_x, &mouse_y); //Adjunta unas coordenadas al mouse

                        //Dibuja la imagen
                        SDL_RenderCopy(rend, texture, NULL, NULL);
                        SDL_RenderPresent(rend);

                        while(SDL_PollEvent(&event))
                            {
                                if(event.type==SDL_QUIT) //Permite salir de la ventana con la cruceta
                                    {
                                        stage=1;
                                        SDL_DestroyTexture(texture);
                                    }

                                     if(mouse_x < 290 && mouse_y > 450 && mouse_x > 60 && mouse_y < 540)
                                {
                                    if(event.type==SDL_MOUSEBUTTONUP) //SONIDO
                                    {
                                        stage=4;
                                        sonido=1;
                                        SDL_DestroyTexture(texture);
                                    }
                                }
                                if(mouse_x > 400 && mouse_y > 450 && mouse_x < 630 && mouse_y < 540) //Selecciona en que parte de la pantalla puedo clickar
                                {
                                    if(event.type==SDL_MOUSEBUTTONUP) //PERSONAJE
                                    {
                                        stage=6;
                                        SDL_DestroyTexture(texture);
                                    }
                                }
                                if ( mouse_x > 710 && mouse_x < 900 && mouse_y > 450 && mouse_y < 540)
                                {
                                    if(event.type==SDL_MOUSEBUTTONUP) //USUARIO
                                    {
                                        stage=7;
                                        SDL_DestroyTexture(texture);
                                    }
                                }
                                 if ( mouse_x > 830 && mouse_x < 980 && mouse_y > 575 && mouse_y < 600)
                                {
                                    if(event.type==SDL_MOUSEBUTTONUP) //VOLVER
                                    {
                                        stage=0;
                                        SDL_DestroyTexture(texture);

                                    }
                                }
                            }
                    }
                }
            break;
            case 6://PERSONAJE
                    while(stage==6){
                            SDL_Surface*surface = IMG_Load("resources/personaje1.jpg");
                            SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, surface); //A�ADE LA NUEVA IMAGEN
                            SDL_FreeSurface(surface);
                            SDL_DestroyTexture(texture);

                        while(stage==6){
                                if (personaje == 1)
                        {
                            SDL_Surface*surface = IMG_Load("resources/personaje1.jpg");
                            SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, surface); //A�ADE LA NUEVA IMAGEN
                            SDL_FreeSurface(surface);

                        }
                    if (personaje == 2)
                    {
                        SDL_Surface*surface = IMG_Load("resources/personaje2.jpg");
                        SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, surface); //A�ADE LA NUEVA IMAGEN
                        SDL_FreeSurface(surface);

                    }
                    else
                        {
                            SDL_Surface*surface = IMG_Load("resources/personaje3.jpg");
                            SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, surface); //A�ADE LA NUEVA IMAGEN
                            SDL_FreeSurface(surface);
                        }
                            buttons = SDL_GetMouseState(&mouse_x, &mouse_y); //Adjunta unas coordenadas al mouse

                        //Dibuja la imagen
                        SDL_RenderCopy(rend, texture, NULL, NULL);
                        SDL_RenderPresent(rend);

                            while(SDL_PollEvent(&event))
                                {
                                    if(event.type==SDL_QUIT) //Permite salir de la ventana si se cierra arriba a la derecha
                                    {
                                        stage=1;
                                        SDL_DestroyTexture(texture);
                                    }
                                    if ( mouse_x > 130 && mouse_x < 315 && mouse_y > 275 && mouse_y < 520)
                                    {
                                       if(event.type==SDL_MOUSEBUTTONUP) //PRIMER PERSONAJE
                                    {
                                        personaje = 1;
                                        SDL_DestroyTexture(texture);
                                        SDL_Surface*surface = IMG_Load("resources/personaje1.jpg");
                                        SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, surface); //A�ADE LA NUEVA IMAGEN
                                        SDL_FreeSurface(surface);
                                        //Dibuja la imagen
                                        SDL_RenderCopy(rend, texture, NULL, NULL);
                                        SDL_RenderPresent(rend);

                                    }
                                    }
                                    if ( mouse_x > 390 && mouse_x < 570 && mouse_y > 275 && mouse_y < 520)
                                    {
                                       if(event.type==SDL_MOUSEBUTTONUP) //SEGUNDO PERSONAJE
                                    {
                                        personaje = 2;
                                        SDL_DestroyTexture(texture);
                                        SDL_Surface*surface = IMG_Load("resources/personaje2.jpg");
                                        SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, surface); //A�ADE LA NUEVA IMAGEN
                                        SDL_FreeSurface(surface);
                                        //Dibuja la imagen
                                        SDL_RenderCopy(rend, texture, NULL, NULL);
                                        SDL_RenderPresent(rend);
                                    }
                                    }
                                    if ( mouse_x > 640 && mouse_x < 820 && mouse_y > 275 && mouse_y < 520)
                                    {
                                       if(event.type==SDL_MOUSEBUTTONUP) //TERCER PERSONAJE
                                    {
                                        personaje = 3;
                                        SDL_DestroyTexture(texture);
                                        SDL_Surface*surface = IMG_Load("resources/personaje3.jpg");
                                        SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, surface); //A�ADE LA NUEVA IMAGEN
                                        SDL_FreeSurface(surface);
                                        //Dibuja la imagen
                                        SDL_RenderCopy(rend, texture, NULL, NULL);
                                        SDL_RenderPresent(rend);
                                    }
                                    }

                                    if ( mouse_x > 830 && mouse_x < 980 && mouse_y > 575 && mouse_y < 600)
                                    {
                                        if(event.type==SDL_MOUSEBUTTONUP) //VOLVER
                                    {
                                        stage=0;
                                        SDL_DestroyTexture(texture);

                                    }
                                    }

                                }

                        }
                }

            break;
            case 7://USUARIO
                    while(stage==7){
                        SDL_Surface*surface = IMG_Load("resources/Proximamente.jpg");
                        SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, surface); //A�ADE LA NUEVA IMAGEN
                        SDL_FreeSurface(surface);

                        while(stage==7){
                            buttons = SDL_GetMouseState(&mouse_x, &mouse_y); //Adjunta unas coordenadas al mouse

                        //Dibuja la imagen
                        SDL_RenderCopy(rend, texture, NULL, NULL);
                        SDL_RenderPresent(rend);

                            while(SDL_PollEvent(&event))
                                {
                                    if(event.type==SDL_QUIT) //Permite salir de la ventana si se cierra arriba a la derecha
                                    {
                                        stage=1;
                                        SDL_DestroyTexture(texture);
                                    }
                                }
                    }
                }
            break;
    }
    }

    //Elimina todos los objetos de SDL
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(mainWin);
    if (sonido == 1)
    {
       SDL_CloseAudioDevice(deviceId);
       SDL_FreeWAV(wavBuffer);
    }
	SDL_Quit();
    return 0;*/



}
