#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_audio.h>


int main(int argc, char *argv[])
{

    int stage=0;  //Indica en que fase del flowchart estamos
    int sonido = 0; //Indica si el sonido se encuentra habilitado o no

    // Se inicializa SDL, con todos los subsistemas y se comprueba si da error
    if(SDL_Init(SDL_INIT_EVERYTHING)!=0)
    {
        printf("Error al iniciar SDL: %s\n", SDL_GetError());
        return 1;
    }

    // Para cargar un archivo de audio, el formato reconocido por la librería básica de SDL es WAV. El clip de audio es cargado:
    SDL_AudioSpec wavSpec;
	Uint32 wavLength;
	Uint8 *wavBuffer;

	SDL_LoadWAV("resources/music2.wav", &wavSpec, &wavBuffer, &wavLength);

	// Con el siguiente comando se abre el dispositivo de audio que lo va a reproducir

	SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);

	// Los argumentos del comando anterior hacen referencia, el primero al nombre del dispositivo que lo reproducirá (al pasarle NULL
    // tomará el dispositivo predeterminado, el segundo es relevante en relación a dispositivos de grabación, no de reproducción, el
    // tercero representa el formato del audio de entrada, el cuarto el formato del audio de salida y el quinto se refiere a escenarios avanzados de audio

	// Reproducir la pista

	int success = SDL_QueueAudio(deviceId, wavBuffer, wavLength); // SDL_QueueAudio permite enviar la información del WAV directamente al dispositivo

    SDL_Window* mainWin = SDL_CreateWindow("HeavyWork", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1000, 650, 0); //Despliegue de ventana HeavyWork, centrada en "x" e "y", de dimensiones 1000x650 y de 0 flags
    //Comprueba que se crea correctamente la ventana
    if(!mainWin)
    {
        printf("Error al crear la ventana: %s\n", SDL_GetError());
        SDL_Quit();
        return 0;
    }

    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC ; //Genera las opciones para el renderizador

    SDL_Renderer* rend = SDL_CreateRenderer(mainWin, -1, render_flags); //Genera el render, se utilizan ambas flags para acudir al software gráfico y para hacer la imagen más fluida evitando superposición de píxeles (screen terror), respectivamente

    if(!rend)
    {
        printf("Error creando renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(mainWin);
        SDL_Quit();
        return 0;
    }



    SDL_Event event; //Creamos una variable de tipo evento
    int SDL_CaptureMouse(SDL_bool enabled);

 while(stage!=1)
    {
        if(sonido==0)
        {
        SDL_PauseAudioDevice(deviceId, 0); //Pausa la grabación de audio al darle un número != 0
        }
        if(sonido==1)
        {
        SDL_PauseAudioDevice(deviceId, 1); //Pausa la grabación de audio al darle un número != 0
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
                            if (sonido == 0)
                            {
                            stage=4;
                            SDL_DestroyTexture(texture);
                            }
                            if (sonido == 1)
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
                    SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, surface); //AÑADE LA NUEVA IMAGEN
                    SDL_FreeSurface(surface);


                    while(stage==2){
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
        case 3://SCORES
                while(stage==3){
                    SDL_Surface*surface = IMG_Load("resources/Proximamente.jpg");
                    SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, surface); //AÑADE LA NUEVA IMAGEN
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
                    SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, surface); //AÑADE LA NUEVA IMAGEN
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
        case 5://SONIDO

                while(stage==5){
                    SDL_Surface*surface = IMG_Load("resources/ajustessinson.jpg");
                    SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, surface); //AÑADE LA NUEVA IMAGEN
                    SDL_FreeSurface(surface);
                    // Parar la música


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
            case 6://PERSONAJE
                    while(stage==6){
                        SDL_Surface*surface = IMG_Load("resources/Proximamente.jpg");
                        SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, surface); //AÑADE LA NUEVA IMAGEN
                        SDL_FreeSurface(surface);

                        while(stage==6){
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
            case 7://USUARIO
                    while(stage==7){
                        SDL_Surface*surface = IMG_Load("resources/Proximamente.jpg");
                        SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, surface); //AÑADE LA NUEVA IMAGEN
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
    return 0;

}
