

#include "../include/player.h"

//Draws player in 2D top down view
void drawPlayer(SDL_Renderer* gRenderer, Player* player){
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 0xFF);
	SDL_Rect playerRect = {player->x, player->y, 8, 8};
	SDL_RenderFillRect(gRenderer, &playerRect);

	SDL_SetRenderDrawColor(gRenderer, 255,255,255,0);
	SDL_RenderDrawLine(gRenderer, player->x+4, player->y+4, player->x+player->deltaX*5+4, player->y+ player->deltaY*5+4);
}

// Handles player movement, takes in direction from DIRECTION enum 
void movePlayer(SDL_Renderer *gRenderer, Player *player, int direction, double rotSpeed){
	// calculate values to check nearest tiles
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
			// Detect for sprite colisions
			int spriteMapX = mapSprites[iPlayerY * mapX + iPlayerXAdd]-1;
			int spriteMapY = mapSprites[iPlayerYAdd * mapX + iPlayerX]-1;

			bool typeCheckX = spriteMapX == -1 ? true : Lsprites[spriteMapX]->type != STATIC_COLIDE;
			bool typeCheckY = spriteMapY == -1 ? true : Lsprites[spriteMapY]->type != STATIC_COLIDE;


			//Check if can move in the X axis to nearest tile
			if(mapWalls[iPlayerY * mapX + iPlayerXAdd] == 0 && typeCheckX){
				player->x += player->deltaX*rotSpeed*0.2;
			}
			//Check if can move in the Y axis in nearest tile
			if(mapWalls[iPlayerYAdd * mapX + iPlayerX] == 0  && typeCheckY){
				player->y += player->deltaY*rotSpeed*0.2;
			}
			break;
		}
		case BACKWARDS:{
			// Detect for sprite colisions
			int spriteMapX = mapSprites[iPlayerY * mapX + iPlayerXSub]-1;
			int spriteMapY = mapSprites[iPlayerYSub * mapX + iPlayerX]-1;

			bool typeCheckX = spriteMapX == -1 ? true : Lsprites[spriteMapX]->type != STATIC_COLIDE;
			bool typeCheckY = spriteMapY == -1 ? true : Lsprites[spriteMapY]->type != STATIC_COLIDE;

			if(mapWalls[iPlayerY * mapX + iPlayerXSub] == 0 && typeCheckX){
				player->x -= player->deltaX*rotSpeed*0.2;
			}
			if(mapWalls[iPlayerYSub * mapX + iPlayerX] == 0 && typeCheckY){
				player->y -= player->deltaY*rotSpeed*0.2;
			}
			break;
		} 
		case RIGHT:{
			// New values have to be calculated to take in account rotation
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

			// Detect for sprite colisions
			int spriteMapX = mapSprites[iPlayerY * mapX + iPlayerXAdd] - 1;
			int spriteMapY = mapSprites[iPlayerYAdd * mapX + iPlayerX]-1;

			bool typeCheckX = spriteMapX == -1 ? true : Lsprites[spriteMapX]->type != STATIC_COLIDE;
			bool typeCheckY = spriteMapY == -1 ? true : Lsprites[spriteMapY]->type != STATIC_COLIDE;

			if(mapWalls[iPlayerY * mapX + iPlayerXAdd] == 0 && typeCheckX){
				player->x += playerDeltaX_temp*0.5 *0.2*rotSpeed;
			}
			if(mapWalls[iPlayerYAdd * mapX + iPlayerX] == 0 && typeCheckY){
				player->y += playerDeltaY_temp*0.5 *0.2 * rotSpeed;
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

			// Detect for sprite colisions
			int spriteMapX = mapSprites[iPlayerY * mapX + iPlayerXAdd] - 1;
			int spriteMapY = mapSprites[iPlayerYAdd * mapX + iPlayerX]-1;

			bool typeCheckX = spriteMapX == -1 ? true : Lsprites[spriteMapX]->type != STATIC_COLIDE;
			bool typeCheckY = spriteMapY == -1 ? true : Lsprites[spriteMapY]->type != STATIC_COLIDE;

			if(mapWalls[iPlayerY * mapX + iPlayerXAdd] == 0 && typeCheckX){
				player->x += playerDeltaX_temp*0.5 * 0.2 * rotSpeed;
			}
			if(mapWalls[iPlayerYAdd * mapX + iPlayerX] == 0 && typeCheckY){
				player->y += playerDeltaY_temp*0.5 * 0.2 * rotSpeed;
			}
		}
    }

}

//Player angle changes handler function
void playerLook(Player *player, int direction, double rotSpeed){
    if(direction == LEFT){
		//Add angle in relation to fps
        player->angle = fixAngle(player->angle +0.01 * rotSpeed*0.2);

		//Calculate new delta X and Y values
        player->deltaX = cos(player->angle);
        player->deltaY = -sin(player->angle);
    }
    else{
        player->angle = fixAngle(player->angle -0.01 * rotSpeed*0.2);
        player->deltaX = cos(player->angle);
        player->deltaY = -sin(player->angle);
    }
}

// Function to check and open doors
void openDoor(Player *player){
	// Check tile in front for door (Value index as 4 in map array)
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

	//If the value in the map array is 4, set it to 0 "opening" the door
    if(mapWalls[iPlayerYAdd*mapX + iPlayerXAdd] == 4)
        mapWalls[iPlayerYAdd*mapX + iPlayerXAdd] = 0;
}


