#ifndef _UTILS_H
#define _UTILS_H

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define PI 3.1415926535
#define PI2 PI/2
#define PI3 3*PI/2
#define DEGREE 0.0174533

//Calculates euclidian distance between two points
float distance(float ax, float ay, float bx, float by);

// Fixes angle given, i.e if over 2PI, reset it back subracting 2PI and inverse for over 0
float fixAngle(float angle);

// Max between 2 floats
float max(float x, float y);

// Min between 2 floats
float min(float x,float y);

float simetricalAngle(float angle);

float distanceAngle(float angle, float ax, float ay, float bx, float by);

bool is_perpendicular(float ax, float ay, float bx, float by);

//TIMER AND TIMER FUNCS

typedef struct{
    Uint32 startTicks;
    Uint32 stopTicks;
    bool paused;
    bool started;
}Timer;

Timer timer(Uint32 startTicks, Uint32 stopTicks, bool paused, bool started);

void startTimer(Timer* mTimer);

void stopTimer(Timer *mTimer);

void pauseTimer(Timer *mTimer);

void unpauseTimer(Timer *mTimer);

Uint32 getTimerTicks(Timer *mTimer);

// TEXTURE AND TEXTURE FUNCS

typedef struct{
    SDL_Texture* texture;
    void* mPixels;
	int mPitch;
    int width;
    int height;
}LTexture;

LTexture* loadFromFile(const char *path, SDL_Renderer* gRenderer,SDL_Window* gWindow);

LTexture* ltexture(SDL_Texture* texture, void*mPixels, int mPitch, int width, int height);

void renderTexture(SDL_Renderer* gRenderer, LTexture* texture,int x, int y, SDL_Rect* clip,int width, int height);

void renderTextureF(SDL_Renderer* gRenderer,LTexture* mtexture, float x, float y, SDL_Rect* clip, float width, float height);

void lockTexture(LTexture* texture);

void unlockTexture(LTexture* texture);

void freeTexture(LTexture* texture);


#endif