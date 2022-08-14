#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "../include/raycast.h"

void drawRays(SDL_Renderer *gRenderer, Player *player){
	int distanceTotal;
	float rayX;
	float rayY;
	float horizontalX;
	float horizontalY;
	float distanceH;
	float distanceV;
	float verticalX;
	float verticalY;
	int verticalMapText;
	int horizontalMapText;

	float rayAngle = fixAngle(player->angle - DEGREE*30);

	for(int i = 0; i < 60;i++){
		verticalMapText = 0;
		horizontalMapText= 0;

		horizontalX = player->x;
		horizontalY = player->y;

		verticalX = player->x;
		verticalY = player->y;

		distanceH = renderHorizontalRays(player, rayAngle, &horizontalX, &horizontalY, &rayX, &rayY, &horizontalMapText);
		distanceV = renderVerticalRays(player, rayAngle, &verticalX, &verticalY, &rayX, &rayY, &verticalMapText);

		float shading = 1;
		if(distanceV < distanceH){
			horizontalMapText = verticalMapText;
			shading = 0.5;
			rayX = verticalX;
			rayY = verticalY;
			distanceTotal = sqrt(distanceV);
			SDL_SetRenderDrawColor(gRenderer, 240, 0, 0, 0xFF);
		}
		if(distanceH < distanceV){
			rayX = horizontalX;
			rayY = horizontalY;
			distanceTotal = sqrt(distanceH);
			SDL_SetRenderDrawColor(gRenderer, 100, 0, 0, 0xFF);
		}

		SDL_RenderDrawLine(gRenderer, player->x+4, player->y+4, rayX, rayY);

		//Draw 3D
		draw3D(gRenderer, player, rayAngle, distanceTotal, horizontalMapText,shading, rayX, rayY,i);
	
		rayAngle = fixAngle(rayAngle + DEGREE);
	}
}
float renderHorizontalRays(Player *player, float rayAngle, float *horizontalX, float *horizontalY, float *rayX, float *rayY, int *horizontalMapText){
	int dof = 0;
	float x0 = 0;
	float y0 = 0;
	int mx;
	int my;
	float distanceH = 10000000000;
	int mapPos;


	float aTan = -1/tan(rayAngle); 
	if(rayAngle > PI){   //looking down
		*rayY = (((int)player->y>>6)<<6) - 0.0001;
		*rayX = (player->y - (*rayY)) * aTan + player->x;
		y0 -= 64;
		x0 -= y0 * aTan;
	}
	if(rayAngle < PI){   //looking up
		*rayY = (((int)player->y>>6)<<6) + 64;
		*rayX = (player->y - (*rayY)) * aTan + player->x;
		y0 = 64;
		x0 -= y0 * aTan;
	}
	// looking straight left or right
	if(rayAngle == 0 || rayAngle == PI){
		*rayX = player->x;
		*rayY = player->y;
		dof = 8;
	}
	while(dof < 8){
		mx = (int)(*rayX)>>6;
		my = (int)(*rayY)>>6;
		mapPos = my * mapX + mx;
		if(mapPos > 0 && mapPos< mapX*mapY && mapWalls[mapPos] >0){ // hit wall
			*horizontalX = *rayX;
			*horizontalY = *rayY;
			distanceH = distance(player->x, player->y, *horizontalX, *horizontalY);
			dof = 8;
			*horizontalMapText = mapWalls[mapPos]-1;
		}
		else{
			(*rayX) += x0;
			(*rayY) += y0;
			dof++;
		}
	}
	return distanceH;
}

float renderVerticalRays(Player *player, float rayAngle, float *verticalX, float *verticalY, float *rayX, float *rayY, int *verticalMapText){
	int dof = 0;
	float x0 = 0;
	float y0 = 0;
	int mx;
	int my;
	float distanceV = 10000000000;
	int mapPos;

	//check vertical lines
	float nTan = -tan(rayAngle); 
	if(rayAngle > PI2 && rayAngle <PI3){   //looking down
		*rayX = (((int)player->x>>6)<<6) - 0.0001;
		*rayY = (player->x - *rayX) * nTan + player->y;
		x0 -= 64;
		y0 -= x0 * nTan;
	}
	if(rayAngle < PI2 || rayAngle > PI3){   //looking up
		*rayX = (((int)player->x>>6)<<6) + 64;
		*rayY = (player->x - *rayX) * nTan + player->y;
		x0 = 64;
		y0 -= x0 * nTan;
	}
	// looking straight left or right
	if(rayAngle == 0 || rayAngle == PI){
		*rayX = player->x;
		*rayY = player->y;
		dof = 8;
	}
	while(dof < 8){
		mx = (int)(*rayX)>>6;
		my = (int)(*rayY)>>6;
		mapPos = my * mapX + mx;
		if(mapPos > 0 && mapPos< mapX*mapY && mapWalls[mapPos] >0){ // hit wall
			*verticalX = *rayX;
			*verticalY = *rayY;
			distanceV = distance(player->x, player->y, *verticalX, *verticalY);
			dof = 8;
			*verticalMapText = mapWalls[mapPos]-1;
		}
		else{
			(*rayX) += x0;
			(*rayY) += y0;
			dof++;
		}
	}
	return distanceV;
}

void draw3D(SDL_Renderer *gRenderer, Player *player, float rayAngle, int distanceTotal, int horizontalMapText, float shading, float rayX, float rayY, int i){
	//Draw 3D
	float cAngle = player->angle - rayAngle;
	cAngle = fixAngle(cAngle);

	distanceTotal = distanceTotal* cos(cAngle);
	float lineHeight = (blockSize*320)/distanceTotal;
		
	int lineOffset = 160-(lineHeight/2);

	float textureX;
	if(shading == 1){
		SDL_SetTextureColorMod(textures[horizontalMapText]->texture, 255, 255, 255);
		textureX = (int)((rayX)/2.0)%32;
		if(rayAngle < PI){
			textureX = 31-textureX;
		}
	}
	else{
		SDL_SetTextureColorMod(textures[horizontalMapText]->texture, 100, 100, 100);
		textureX = (int)((rayY)/2.0)%32;
		if(rayAngle > PI2 && rayAngle < PI3){
			textureX = 31-textureX;
		}
	}

	// draw textured walls
	SDL_Rect wall = {textureX, 0, 1,32};
	renderTexture(gRenderer,textures[horizontalMapText], i*8+530,lineOffset,&wall, 8, lineHeight);

	//floors SDL_RECT
	for(int j = lineOffset + lineHeight;j < 540;j++){
		SDL_Rect floor = {i*8 + 530, j, 8,1};
		SDL_Rect ceil = {i*8 + 530, 320-j, 8, 1};
		
		SDL_SetRenderDrawColor(gRenderer,127,127,127,255);
		SDL_RenderFillRect(gRenderer, &floor);
		SDL_SetRenderDrawColor(gRenderer,50,50,50,0);
		SDL_RenderFillRect(gRenderer, &ceil);
	}

	/*  TEXTUREDFLOOR ATTEMPT

	for(int j = lineOffset + lineHeight;j < 540;j++){
		// printf("%d\n", j);
		// printf("%f\n", rayAngle);
		float deltaY = j - (320/2.0);
		float degree = rayAngle;
		float rayAngleFix = cos(fixAngle(player->angle - rayAngle));
		textureX = (player->x/2 + (cos(degree)*158*32/deltaY/rayAngleFix));
		float textureY = (player->y/2) - (sin(degree)*158*32/deltaY/rayAngleFix);
		SDL_Rect floor = {(int)textureX%32,(int)textureY % 32, 1,32 };
		int map = mapFloors[mapX+(int)(textureX/32.0)];
		// printf("%d\n", map);
		SDL_SetTextureColorMod(textures[0]->texture, 255, 255, 255);
		renderTexture(gRenderer, textures[0], i*8+530,j,&floor,8,1);

	}

	*/
}

bool checkColisions(SDL_Renderer *gRenderer, Player *player,float directionOffset){

	float rayX;
	float rayY;

	float rayAngle = player->angle + directionOffset;
	rayAngle = fixAngle(rayAngle);

	int verticalMapText = 0;
	int horizontalMapText= 0;

	float horizontalX = player->x;
	float horizontalY = player->y;

	float verticalX = player->x;
	float verticalY = player->y;

	float distanceH = renderHorizontalRays(player,rayAngle, &horizontalX, &horizontalY, &rayX, &rayY, &horizontalMapText);
	float distanceV = renderVerticalRays(player,rayAngle, &verticalX, &verticalY, &rayX, &rayY, &verticalMapText);

	if(distanceV < distanceH){
		horizontalMapText = verticalMapText;
		rayX = verticalX;
		rayY = verticalY;
		SDL_SetRenderDrawColor(gRenderer, 240, 0, 0, 0xFF);
	}
	if(distanceH < distanceV){
		rayX = horizontalX;
		rayY = horizontalY;
		SDL_SetRenderDrawColor(gRenderer, 100, 0, 0, 0xFF);
	}
	return sqrt(distance(player->x, player->y,rayX, rayY)) > 20;
}
