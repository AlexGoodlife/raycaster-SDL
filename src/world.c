#include "../include/world.h"

int mapX = 8;
int mapY= 8;
int blockSize = 64;

int mapWalls[]=          //walls
{
 1,1,1,1,2,4,2,2,
 2,0,0,0,0,0,0,2,
 1,0,0,2,0,0,0,2,
 1,4,2,0,0,0,0,2,
 2,0,0,0,0,0,0,1,
 2,0,0,0,0,0,0,1,
 2,0,2,0,2,0,1,1,
 1,1,1,1,1,1,1,1,	
};

int mapSprites[] =   //Map to determine sprite positions
{
 0,0,0,0,0,0,0,0,
 0,0,0,0,3,4,5,0,
 0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,
 0,1,0,0,0,2,0,0,
 0,0,0,0,0,0,0,0,
 0,0,0,6,0,0,0,0,
 0,0,0,0,0,0,0,0,	
};

// Draws 2D map grid based on wall map
void drawMap2D(SDL_Renderer *gRenderer){
	int x0;
	int y0;
	for(int i = 0; i <mapY;i++){
		for(int j = 0; j < mapX;j++){
			if(mapWalls[i*mapX + j] > 0)
				SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 0xFF);
			else
				SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
			x0 = j * blockSize;
			y0 = i * blockSize;
			SDL_Rect block = {x0, y0, blockSize-1, blockSize-1};
			SDL_RenderFillRect(gRenderer, &block);

		}
	}
}
