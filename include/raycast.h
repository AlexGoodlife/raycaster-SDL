
#ifndef RAYCAST_H
#define RAYCAST_H


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

void drawFloors(SDL_Renderer *gRenderer,int screenHeight, int screenWidth);


typedef struct{
    float x;
    float y;
    float z;
    float playerDist;
    bool state;
    int type;
    LTexture **texture;
    int n_texts;
}Sprite;

extern Sprite *Lsprites[7];

Sprite* sprite(float x, float y, float z, float playerDist, bool state, int type, LTexture **texture, int n_texts);

void freeSprite(Sprite* spr);

#define n_sprites (sizeof Lsprites / sizeof *Lsprites)

void drawSprites(SDL_Renderer *gRenderer,Player *player);



#endif