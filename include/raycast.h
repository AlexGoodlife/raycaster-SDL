
#ifndef RAYCAST_H
#define RAYCAST_H

#include <SDL.h>
#include "world.h"
#include "utils.h"
#include "player.h"


//Raycasting main function
void drawRays(SDL_Renderer *gRenderer, Player *player);

//Handles 3D projection onto screen
void draw3D(SDL_Renderer *gRenderer, Player *player, float rayAngle, int distanceTotal, int horizontalMapText, float shading, float rayX, float rayY,int i);

//Renders vertical Rays 
float renderVerticalRays(Player *player, float rayAngle, float *verticalX, float *verticalY, float *rayX, float *rayY, int *verticalMapText);

//Renders horizontal Rays 
float renderHorizontalRays(Player *player, float rayAngle, float *horizontalX, float *horizontalY, float *rayX, float *rayY, int *horizontalMapText);

//Casts a ray and checks for colisions between the player and the wall the ray hit
bool checkColisions(SDL_Renderer *gRenderer, Player *player,float directionOffset);

#endif