#include "../include/world.h"

int mapX = 8;
int mapY= 8;
int blockSize = 64;

// int mapWalls[] = 
// {
// 	1,1,1,1,1,2,1,1,1,1,
// 	1,0,0,2,0,0,0,0,0,1,
// 	1,2,2,2,0,0,0,0,0,1,
// 	1,0,0,0,0,0,0,0,2,1,
// 	1,0,0,0,0,0,0,0,2,1,
// 	1,0,1,0,0,3,0,0,0,1,
// 	1,0,1,0,0,0,0,0,0,1,
// 	1,0,0,2,2,0,0,0,0,1,
// 	1,0,0,2,2,0,0,0,2,1,
// 	1,1,1,2,2,1,1,1,1,1,
// };

int mapWalls[]=          //walls
{
 1,1,1,1,2,2,2,2,
 2,0,0,4,0,0,0,2,
 1,0,0,2,0,2,0,2,
 1,4,2,2,0,0,0,2,
 2,0,0,0,0,0,0,1,
 2,0,0,0,0,1,0,1,
 2,0,2,0,2,0,0,1,
 1,1,1,1,1,1,1,1,	
};

int mapFloors[]=          //floors
{
 0,0,0,0,0,0,0,0,
 0,1,1,1,1,1,1,0,
 0,1,1,1,1,1,1,0,
 0,1,1,2,2,1,1,0,
 0,1,1,2,2,1,1,0,
 0,1,1,1,1,1,1,0,
 0,1,1,1,1,1,1,0,
 0,0,0,0,0,0,0,0,		
};


int mapCeiling[]=          //ceiling
{
 0,0,0,0,0,0,0,0,
 0,1,1,1,1,1,1,0,
 0,1,1,1,1,1,1,0,
 0,1,1,1,1,1,1,0,
 0,1,1,1,1,1,1,0,
 0,1,1,1,1,1,1,0,
 0,1,1,1,1,1,1,0,
 0,0,0,0,0,0,0,0,	
};

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
