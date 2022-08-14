#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "../include/raycast.h"

void drawRays(SDL_Renderer *gRenderer, float playerAngle, float playerX, float playerY){
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

	float rayAngle = fixAngle(playerAngle - DEGREE*30);

	for(int i = 0; i < 60;i++){
		verticalMapText = 0;
		horizontalMapText= 0;

		horizontalX = playerX;
		horizontalY = playerY;

		verticalX = playerX;
		verticalY = playerY;

		distanceH = renderHorizontalRays(playerX, playerY, rayAngle, &horizontalX, &horizontalY, &rayX, &rayY, &horizontalMapText);
		distanceV = renderVerticalRays(playerX, playerY, rayAngle, &verticalX, &verticalY, &rayX, &rayY, &verticalMapText);

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

		SDL_RenderDrawLine(gRenderer, playerX+4, playerY+4, rayX, rayY);

		//Draw 3D
		draw3D(gRenderer, playerAngle, rayAngle, distanceTotal, horizontalMapText,shading, rayX, rayY, playerX, playerY, i);
	
		rayAngle = fixAngle(rayAngle + DEGREE);
	}
}

float renderHorizontalRays(float playerX, float playerY, float rayAngle, float *horizontalX, float *horizontalY, float *rayX, float *rayY, int *horizontalMapText){
	int dof = 0;
	float x0 = 0;
	float y0 = 0;
	int mx;
	int my;
	float distanceH = 10000000000;
	int mapPos;


	float aTan = -1/tan(rayAngle); 
	if(rayAngle > PI){   //looking down
		*rayY = (((int)playerY>>6)<<6) - 0.0001;
		*rayX = (playerY - (*rayY)) * aTan + playerX;
		y0 -= 64;
		x0 -= y0 * aTan;
	}
	if(rayAngle < PI){   //looking up
		*rayY = (((int)playerY>>6)<<6) + 64;
		*rayX = (playerY - (*rayY)) * aTan + playerX;
		y0 = 64;
		x0 -= y0 * aTan;
	}
	// looking straight left or right
	if(rayAngle == 0 || rayAngle == PI){
		*rayX = playerX;
		*rayY = playerY;
		dof = 8;
	}
	while(dof < 8){
		mx = (int)(*rayX)>>6;
		my = (int)(*rayY)>>6;
		mapPos = my * mapX + mx;
		if(mapPos > 0 && mapPos< mapX*mapY && mapWalls[mapPos] >0){ // hit wall
			*horizontalX = *rayX;
			*horizontalY = *rayY;
			distanceH = distance(playerX, playerY, *horizontalX, *horizontalY);
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

float renderVerticalRays(float playerX, float playerY, float rayAngle, float *verticalX, float *verticalY, float *rayX, float *rayY, int *verticalMapText){
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
		*rayX = (((int)playerX>>6)<<6) - 0.0001;
		*rayY = (playerX - *rayX) * nTan + playerY;
		x0 -= 64;
		y0 -= x0 * nTan;
	}
	if(rayAngle < PI2 || rayAngle > PI3){   //looking up
		*rayX = (((int)playerX>>6)<<6) + 64;
		*rayY = (playerX - *rayX) * nTan + playerY;
		x0 = 64;
		y0 -= x0 * nTan;
	}
	// looking straight left or right
	if(rayAngle == 0 || rayAngle == PI){
		*rayX = playerX;
		*rayY = playerY;
		dof = 8;
	}
	while(dof < 8){
		mx = (int)(*rayX)>>6;
		my = (int)(*rayY)>>6;
		mapPos = my * mapX + mx;
		if(mapPos > 0 && mapPos< mapX*mapY && mapWalls[mapPos] >0){ // hit wall
			*verticalX = *rayX;
			*verticalY = *rayY;
			distanceV = distance(playerX, playerY, *verticalX, *verticalY);
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

void draw3D(SDL_Renderer *gRenderer, float playerAngle, float rayAngle, int distanceTotal, int horizontalMapText, float shading, float rayX, float rayY, int playerX, int playerY, int i){
	//Draw 3D
	float cAngle = playerAngle - rayAngle;
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
		float rayAngleFix = cos(fixAngle(playerAngle - rayAngle));
		textureX = (playerX/2 + (cos(degree)*158*32/deltaY/rayAngleFix));
		float textureY = (playerY/2) - (sin(degree)*158*32/deltaY/rayAngleFix);
		SDL_Rect floor = {(int)textureX%32,(int)textureY % 32, 1,32 };
		int map = mapFloors[mapX+(int)(textureX/32.0)];
		// printf("%d\n", map);
		SDL_SetTextureColorMod(textures[0]->texture, 255, 255, 255);
		renderTexture(gRenderer, textures[0], i*8+530,j,&floor,8,1);

	}

	*/
}

bool checkColisions(SDL_Renderer *gRenderer, float playerAngle, float playerX, float playerY,float directionOffset){

	float rayX;
	float rayY;

	float rayAngle = playerAngle + directionOffset;
	rayAngle = fixAngle(rayAngle);

	int verticalMapText = 0;
	int horizontalMapText= 0;

	float horizontalX = playerX;
	float horizontalY = playerY;

	float verticalX = playerX;
	float verticalY = playerY;

	float distanceH = renderHorizontalRays(playerX, playerY, rayAngle, &horizontalX, &horizontalY, &rayX, &rayY, &horizontalMapText);
	float distanceV = renderVerticalRays(playerX, playerY, rayAngle, &verticalX, &verticalY, &rayX, &rayY, &verticalMapText);

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
	return sqrt(distance(playerX, playerY,rayX, rayY)) > 20;
}









