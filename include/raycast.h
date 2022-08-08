
#ifndef RAYCAST_H
#define RAYCAST_H

#include <SDL.h>
#include "world.h"
#include "utils.h"




void drawRays(SDL_Renderer *gRenderer, float playerAngle, int playerX, int playerY);

void draw3D(SDL_Renderer *gRenderer, float playerAngle, float rayAngle, int distanceTotal, int horizontalMapText, float shading, float rayX, float rayY, int playerX, int playerY, int i);

float renderVerticalRays(int playerX, int playerY, float rayAngle, float *verticalX, float *verticalY, float *rayX, float *rayY, int *verticalMapText);

float renderHorizontalRays(int playerX, int playerY, float rayAngle, float *horizontalX, float *horizontalY, float *rayX, float *rayY, int *horizontalMapText);

bool checkColisions(SDL_Renderer *gRenderer, float playerAngle, int playerX, int playerY, int playerDeltaX, int playerDeltaY, float directionOffset);

extern int smallestRay;


#endif