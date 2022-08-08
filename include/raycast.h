
#ifndef RAYCAST_H
#define RAYCAST_H

#include <SDL.h>
#include "world.h"
#include "utils.h"


//Raycasting main function
void drawRays(SDL_Renderer *gRenderer, float playerAngle, int playerX, int playerY);

//Handles 3D projection onto screen
void draw3D(SDL_Renderer *gRenderer, float playerAngle, float rayAngle, int distanceTotal, int horizontalMapText, float shading, float rayX, float rayY, int playerX, int playerY, int i);

//Renders vertical Rays 
float renderVerticalRays(int playerX, int playerY, float rayAngle, float *verticalX, float *verticalY, float *rayX, float *rayY, int *verticalMapText);

//Renders horizontal Rays 
float renderHorizontalRays(int playerX, int playerY, float rayAngle, float *horizontalX, float *horizontalY, float *rayX, float *rayY, int *horizontalMapText);

//Casts a ray and checks for colisions between the player and the wall the ray hit, 
//Currently only used for checking colisions in the players back as the front is handled in the main raycasting function
bool checkColisions(SDL_Renderer *gRenderer, float playerAngle, int playerX, int playerY, int playerDeltaX, int playerDeltaY, float directionOffset);

//Variable used for frontal player colisions
extern int smallestRay;


#endif