

#include "../include/player.h"

void drawPlayer(SDL_Renderer* gRenderer, Player* player){
	// Drawing player
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 0xFF);
	SDL_Rect playerRect = {player->x, player->y, 8, 8};
	SDL_RenderFillRect(gRenderer, &playerRect);

	SDL_SetRenderDrawColor(gRenderer, 255,255,255,0);
	SDL_RenderDrawLine(gRenderer, player->x+4, player->y+4, player->x+player->deltaX*5+4, player->y+ player->deltaY*5+4);
}

void movePlayer(SDL_Renderer *gRenderer, Player *player, int direction, double rotSpeed){
    int xOffset = 0;
	int yOffset = 0;
	if(player->deltaX < 0)
		xOffset -= 20;
	else
		xOffset = 20;
	if(player->deltaY < 0)
		yOffset -= 20;
	else
		yOffset = 20;
	int iPlayerX = (int)player->x >> 6; // bitshift right by 6 to divide by 64
	int iPlayerXAdd = (int)(player->x + xOffset) >> 6;
	int iPlayerXSub = (int)(player->x - xOffset) >> 6;

	int iPlayerY = (int)player->y >> 6;
	int iPlayerYAdd = (int)(player->y + yOffset) >> 6;
	int iPlayerYSub = (int)(player->y - yOffset) >> 6;
    switch (direction)
    {
    case FORWARD:{
        if(mapWalls[iPlayerY * mapX + iPlayerXAdd] == 0){
		    player->x += player->deltaX*rotSpeed*0.2; //* timeStep; //*1/avgFPS;
	    }
        if(mapWalls[iPlayerYAdd * mapX + iPlayerX] == 0){
		    player->y += player->deltaY*rotSpeed*0.2; //*timeStep; //* 1/avgFPS;
	    }
        break;
	}
    case BACKWARDS:{
        if(mapWalls[iPlayerY * mapX + iPlayerXSub] == 0){
			player->x -= player->deltaX*rotSpeed*0.2; //*timeStep; //* 0.2 * fps;
		}
	    if(mapWalls[iPlayerYSub * mapX + iPlayerX] == 0){
			player->y -= player->deltaY*rotSpeed*0.2; //*timeStep; //* 0.2 * fps;
		}
        break;
	} 
    case RIGHT:{
		xOffset = 0;
		yOffset = 0;
		float playerDeltaX_temp = cos(fixAngle(player->angle - PI/2));
        float playerDeltaY_temp = -sin(fixAngle(player->angle - PI/2));
		if(playerDeltaX_temp < 0)
			xOffset -= 20;
		else
		    xOffset = 20;
		if(playerDeltaY_temp < 0)
			yOffset -= 20;
		else
			yOffset = 20;
		int iPlayerXAdd = (int)(player->x + xOffset) >> 6;

		int iPlayerYAdd = (int)(player->y + yOffset) >> 6;
        if(mapWalls[iPlayerY * mapX + iPlayerXAdd] == 0){
			player->x += playerDeltaX_temp*0.5 *0.2*rotSpeed;// * timeStep; //*1/avgFPS;
		}
		if(mapWalls[iPlayerYAdd * mapX + iPlayerX] == 0){
			player->y += playerDeltaY_temp*0.5 *0.2 * rotSpeed;// *timeStep; //* 1/avgFPS;
		}
        break;
	}
    
    case LEFT:{
        xOffset = 0;
		yOffset = 0;
		float playerDeltaX_temp = cos(fixAngle(player->angle + PI/2));
        float playerDeltaY_temp = -sin(fixAngle(player->angle+ PI/2));
		if(playerDeltaX_temp < 0)
			xOffset -= 20;
		else
		xOffset = 20;
		if(playerDeltaY_temp < 0)
			yOffset -= 20;
		else
			yOffset = 20;
		iPlayerXAdd = (int)(player->x + xOffset) >> 6;

		iPlayerYAdd = (int)(player->y + yOffset) >> 6;
        if(mapWalls[iPlayerY * mapX + iPlayerXAdd] == 0){
			player->x += playerDeltaX_temp*0.5 * 0.2 * rotSpeed; //* timeStep; //*1/avgFPS;
		}
		if(mapWalls[iPlayerYAdd * mapX + iPlayerX] == 0){
			player->y += playerDeltaY_temp*0.5 * 0.2 * rotSpeed; //*timeStep; //* 1/avgFPS;
		}
	}
    }

}

void playerLook(Player *player, int direction, double rotSpeed){
	// printf("%f\n", rotSpeed);
    if(direction == LEFT){
        player->angle = fixAngle(player->angle +0.01 * rotSpeed*0.2);
        player->deltaX = cos(player->angle);
        player->deltaY = -sin(player->angle);
    }
    else{
        player->angle = fixAngle(player->angle -0.01 * rotSpeed*0.2);
        player->deltaX = cos(player->angle);
        player->deltaY = -sin(player->angle);
    }
}

// void updatePlayer(double rotSpeed){
// 	player->angle = fixAngle(player->angle + 0.05 * rotSpeed);
// 	player->deltaX = cos(player->angle)*5;
// 	player->deltaY = -sin(player->angle)*5;
// 	rotSpeed-= 0.05;
// }

void openDoor(Player *player){
    int xOffset = 0;
    int yOffset = 0;
    if(player->deltaX < 0)
        xOffset -= 25;
    else
        xOffset = 25;
    if(player->deltaY < 0)
        yOffset -= 25;
    else
        yOffset = 25;
    int iPlayerXAdd = (int)(player->x + xOffset) >> 6; // bitshift right to divide by 64

    int iPlayerYAdd = (int)(player->y + yOffset) >> 6;

    if(mapWalls[iPlayerYAdd*mapX + iPlayerXAdd] == 4)
        mapWalls[iPlayerYAdd*mapX + iPlayerXAdd] = 0;
}


