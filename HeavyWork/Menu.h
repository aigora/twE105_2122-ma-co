#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_audio.h>
#include "Utilities.h"
#define SPEED 300//velocidad en pixeles por segundo

int menu(Window window, Textures tex, int *personaje, int stage, int sonido);
