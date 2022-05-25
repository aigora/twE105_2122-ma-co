#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
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
    SDL_Texture* ayuda;
    SDL_Texture* carga;
    SDL_Texture* wall;
    SDL_Texture* fondo;
    SDL_Texture* player;
    SDL_Texture* playerinv;
    SDL_Texture* playerdrcha;
    SDL_Texture* playerizqda;
    SDL_Texture* playeratras;
    SDL_Texture* playerinvatras;
    SDL_Texture* playerinvdrcha;
    SDL_Texture* playerinvizqda;
    SDL_Texture* prox;
    SDL_Texture* bot;
    SDL_Texture* vida;
    SDL_Texture* pause;
    SDL_Texture* billete;
    SDL_Texture* cafe;
    SDL_Texture* charco;
    SDL_Texture* salida;
    SDL_Texture* creditos;
    SDL_Texture* titulo_puntuacion;
    SDL_Texture* boton;
    SDL_Texture* escaleras;

} Textures; //Estructura de las texturas

typedef struct
{
    SDL_Texture* tex;
    SDL_Rect src;
    SDL_Rect dst;

} Entity; //Estructura de los muros Y tokens

typedef struct
{
    Entity entity;
    bool collected; //Solo para Tokens
    int type; //Solo para tipo token

} Tokens;

typedef struct
{
    Entity entity;

} Bot;

typedef struct {

    bool W;
    bool A;
    bool S;
    bool D;
    bool SPACE;
    bool ESC;
    bool ESC_PREV;
} key_buttons;

void pushStack(Stack stack[],int x, int y);
int popStack(Stack stack[]);
SDL_Texture* loadTexture(const char p_file[], Window window);
void renderFondo(Window window, SDL_Texture* tex);
void renderPause(Window window, SDL_Texture* tex);
void imprimirImagen(Window window, SDL_Texture* tex);
int ComprobarMuros(int new_x, int new_y, Vector2f v2, const Entity* muros, int num_muros);
bool UpdateKeys(key_buttons* k, SDL_Event event, bool* game, bool* update);
bool boton_invisibilidad (bool boton, float game_time, float *tiempo_boton_in, float *tiempo_boton_fin, float *tiempo_fin_invisibilidad, bool *invisibilidad, Mix_Chunk *invisi);
int max(int v[],int n);


