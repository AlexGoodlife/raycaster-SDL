#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "../include/raycast.h"

int smallestRay = INT32_MAX;

void drawRays(SDL_Renderer *gRenderer, float playerAngle, int playerX, int playerY){
	smallestRay = INT32_MAX;
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

	float rayAngle = playerAngle - DEGREE*30;
	rayAngle = fixAngle(rayAngle);

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
			distanceTotal = distanceV;
			SDL_SetRenderDrawColor(gRenderer, 240, 0, 0, 0xFF);
		}
		if(distanceH < distanceV){
			rayX = horizontalX;
			rayY = horizontalY;
			distanceTotal = distanceH;
			SDL_SetRenderDrawColor(gRenderer, 100, 0, 0, 0xFF);
		}

		SDL_RenderDrawLine(gRenderer, playerX+4, playerY+4, rayX, rayY);
		smallestRay = min((float)smallestRay, (float)distanceTotal);

		//Draw 3D
		draw3D(gRenderer, playerAngle, rayAngle, distanceTotal, horizontalMapText,shading, rayX, rayY, playerX, playerY, i);
	
		rayAngle += DEGREE;
		rayAngle = fixAngle(rayAngle);
	}
}

float renderHorizontalRays(int playerX, int playerY, float rayAngle, float *horizontalX, float *horizontalY, float *rayX, float *rayY, int *horizontalMapText){
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

float renderVerticalRays(int playerX, int playerY, float rayAngle, float *verticalX, float *verticalY, float *rayX, float *rayY, int *verticalMapText){
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
		
		float textureYStep = 32.0 /(float)lineHeight;
		float textureYOffset = 0;
		if(lineHeight >320){
			textureYOffset = (lineHeight-320)/2;
			lineHeight = 320;
		}

		int lineOffset = 160-(lineHeight/2);

		float textureY = textureYOffset * textureYStep + horizontalMapText*32;
		float textureX;
		if(shading == 1){
			textureX = (int)((rayX)/2.0)%32;
			if(rayAngle < PI){
				textureX = 31-textureX;
			}
		}
		else{
			textureX = (int)((rayY)/2.0)%32;
			if(rayAngle > PI2 && rayAngle < PI3){
				textureX = 31-textureX;
			}
		}
		for(int j = 0; j < lineHeight;j++)
		{
			float color = textures[(int)(textureY)*32 + (int)(textureX)]*shading;
			SDL_SetRenderDrawColor(gRenderer, color*255, color*255,color*255, 255);
			for(int line = 0; line < 8;line++)
				SDL_RenderDrawPoint(gRenderer, i*8+line+530,j+lineOffset);
			textureY += textureYStep;
		}

		// Draw Floors
		for(int j = lineOffset + lineHeight; j <320;j++){
			float deltaY = j - (320/2.0);
			float degree = rayAngle;
			float rayAngleFix = cos(fixAngle(playerAngle - rayAngle));
			textureX = playerX/2 + cos(degree)*158*32/deltaY/rayAngleFix;
			textureY = playerY/2 - sin(degree)*158*32/deltaY/rayAngleFix;
			
			int map = mapFloors[(int)(textureY/32.0)*mapX+(int)(textureX/32.0)]*32*32;
			float color = textures[((int)(textureY)&31)*32 + ((int)(textureX)&31)+map] *0.7;
			SDL_SetRenderDrawColor(gRenderer, color/1.3*255, color/1.3*255,color*255, 255);

			for(int line = 0; line < 8;line++){
				SDL_RenderDrawPoint(gRenderer, i*8+530+line,j);
			}
			
			//DRAW CEILING
			map = mapCeiling[(int)(textureY/32.0)*mapX+(int)(textureX/32.0)]*32*32;
			color = textures[((int)(textureY)&31)*32 + ((int)(textureX)&31)+map] *0.7;
			SDL_SetRenderDrawColor(gRenderer, color/2.0*255, color/1.2*255,color/2.0*255, 255);
			
			for(int line = 0; line < 8;line++){
				SDL_RenderDrawPoint(gRenderer, i*8+line+530,320 - j);
			}

		}
}

bool checkColisions(SDL_Renderer *gRenderer, float playerAngle, int playerX, int playerY, int playerDeltaX, int playerDeltaY,float directionOffset){

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
	if(directionOffset >= PI){
		playerDeltaX = -playerDeltaX;
		playerDeltaY = -playerDeltaY;
	
	}

	return distance(playerX, playerY,rayX, rayY) > 20;
}









