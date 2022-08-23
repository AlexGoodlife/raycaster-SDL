#ifndef _PLAYER_H
#define _PLAYER_H

#include "utils.h"

//Struct to hold player information
typedef struct{
    float x;
    float y;
    float deltaX;
    float deltaY;
    float angle;
}Player;

enum DIRECTION {FORWARD, BACKWARDS, RIGHT, LEFT};

void drawPlayer(SDL_Renderer* gRenderer, Player* player);

void movePlayer(SDL_Renderer *gRenderer, Player *player, int direction,double rotSpeed);

void playerLook(Player *player, int direction, double rotSpeed);

void openDoor(Player *player);

#endif
