#ifndef WORLD_H
#define WORLD_H

// World textures and maps, textures taken from 3D Sage

#include <SDL.h>
#include "utils.h"

int mapX;
int mapY;
int blockSize;

// Wall map
extern int mapWalls[];

// Floor map
extern int mapFloors[];

// Ceiling map
extern int mapCeiling[];

void drawMap2D(SDL_Renderer *gRenderer);

// LTexture *gSpriteSheetTexture;

SDL_Rect gSprites[4];

LTexture* textures[3];

#define n_textures (sizeof textures / sizeof *textures)

#endif