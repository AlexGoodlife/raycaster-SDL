#ifndef WORLD_H
#define WORLD_H

// World textures and maps, textures taken from 3D Sage

#include <SDL.h>
#define TEXT_FLOORS

extern int mapX;
extern int mapY;
extern int blockSize;

// Wall map
extern int mapWalls[];

extern int mapSprites[];

extern int mapFloors[];

extern int mapCeiling[];

void drawMap2D(SDL_Renderer *gRenderer);

#endif