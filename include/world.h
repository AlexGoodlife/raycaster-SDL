#ifndef WORLD_H
#define WORLD_H

// World textures and maps, textures taken from 3D Sage

#include <SDL.h>
#include "utils.h"

extern int mapX;
extern int mapY;
extern int blockSize;

// Wall map
extern int mapWalls[];

// Floor map
extern int mapFloors[];

// Ceiling map
extern int mapCeiling[];

void drawMap2D(SDL_Renderer *gRenderer);

extern LTexture* textures[4];

#define n_textures (sizeof textures / sizeof *textures)

#endif