#ifndef WORLD_H
#define WORLD_H

// World textures and maps, textures taken from 3D Sage

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

LTexture *gSpriteSheetTexture;

SDL_Rect gSprites[4];

LTexture* textures[3];

#define n_textures (sizeof textures / sizeof *textures)

#endif