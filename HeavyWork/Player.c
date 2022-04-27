#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_audio.h>
#include "Player.h"
#include "Utilities.h"

float modist(Vector2f v1, Vector2f v2)//V1=player v2=IA
{
    float mod;
    mod = sqrt(pow((v2.x - v1.x),2) + pow((v2.y - v1.y), 2));
    return mod;
}
