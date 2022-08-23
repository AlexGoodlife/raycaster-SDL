#ifndef WORLD_H
#define WORLD_H

// World textures and maps, textures taken from 3D Sage

#include <SDL.h>

extern int mapX;
extern int mapY;
extern int blockSize;

// Wall map
extern int mapWalls[];

extern int mapSprites[];

void drawMap2D(SDL_Renderer *gRenderer);

#endif