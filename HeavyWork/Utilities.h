#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_audio.h>

typedef struct
{
    float x;
    float y;

} Vector2f; //Vector de dos dimensiones de decimales

typedef struct
{
    int x;
    int y;

} Vector2i; //Vector de dos dimensiones de enteros

typedef struct
{
    int x;
    int y;
    bool last;

} Stack; //Una estructura stack de enteros

typedef struct
{
    int w,h;
    SDL_Window* window;
    SDL_Renderer* renderer;

} Window; //Estructura de ventana

typedef struct
{
    SDL_Texture* menu;
    SDL_Texture* ajustes;
    SDL_Texture* ajusnos;
    SDL_Texture* personaje1;
    SDL_Texture* personaje2;
    SDL_Texture* personaje3;
    SDL_Texture* carga;
    SDL_Texture* wall;
    SDL_Texture* fondo;
    SDL_Texture* player;

} Textures; //Estructura de las texturas

typedef struct
{
    SDL_Texture* tex;
    SDL_Rect src;
    SDL_Rect dst;

} Entity; //Estructura de los muros

void pushStack(Stack stack[],int x, int y);
int popStack(Stack stack[]);
SDL_Texture* loadTexture(const char p_file[], Window window);
void renderLab(Window window, Entity muros[], int nmuros);
void renderFondo(Window window, SDL_Texture* tex);

