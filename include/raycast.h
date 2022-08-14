
#ifndef RAYCAST_H
#define RAYCAST_H

#include <SDL.h>
#include "world.h"
#include "utils.h"


//Raycasting main function
void drawRays(SDL_Renderer *gRenderer, float playerAngle, float playerX, float playerY);

//Handles 3D projection onto screen
void draw3D(SDL_Renderer *gRenderer, float playerAngle, float rayAngle, int distanceTotal, int horizontalMapText, float shading, float rayX, float rayY, int playerX, int playerY, int i);

//Renders vertical Rays 
float renderVerticalRays(float playerX, float playerY, float rayAngle, float *verticalX, float *verticalY, float *rayX, float *rayY, int *verticalMapText);

//Renders horizontal Rays 
float renderHorizontalRays(float playerX, float playerY, float rayAngle, float *horizontalX, float *horizontalY, float *rayX, float *rayY, int *horizontalMapText);

//Casts a ray and checks for colisions between the player and the wall the ray hit
bool checkColisions(SDL_Renderer *gRenderer, float playerAngle, float playerX, float playerY,float directionOffset);

#endif