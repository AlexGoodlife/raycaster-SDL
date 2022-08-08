
#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "include\world.h"


#define PI 3.1415926535
#define PI2 PI/2
#define PI3 3*PI/2
#define DEGREE 0.0174533
#define MAX_VELOCITY 2

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 512;

#define CTR_X (SCREEN_WIDTH / 2)
#define CTR_Y (SCREEN_HEIGHT / 2)


int playerX;
int playerY;
float playerDeltaX;
float playerDeltaY;
float playerAngle;
float mouseX;
float mouseY;
float velocityX = 0;
float velocityY = 0;
Uint32 frame1 = 0;
Uint32 frame2 = 0;
Uint32 fps = 0;



float distance(float ax, float ay, float bx, float by, float angle);

float fixAngle(float angle);

// The window we'll be rendering to
SDL_Window *gWindow = NULL;

// The surface contained by the window
SDL_Surface *gScreenSurface = NULL;

SDL_Renderer *gRenderer = NULL;

bool init()
{
	// Initialization flag
	bool success = true;
	playerX = 300;
	playerY = 300;
	playerAngle = 0;
	playerDeltaX = cos(playerAngle)*5;
	playerDeltaY = sin(playerAngle)*5;
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		// Create window
		gWindow = SDL_CreateWindow("Raycaster", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(gRenderer, 105, 105, 105, 0);
				SDL_SetRelativeMouseMode(SDL_TRUE);
				//SDL_ShowCursor(SDL_DISABLE);
				// Get window surface
				gScreenSurface = SDL_GetWindowSurface(gWindow);
			}
		}
	}
	return success;
}

void close()
{

	// Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gRenderer = NULL;
	gWindow = NULL;

	// Quit SDL subsystems
	SDL_Quit();
}

float max(float x, float y){
	return x > y ? x : y;
}

float min(float x,float y){
	return x < y ? x : y;
}

void movePlayer(){
	playerX += velocityX;
	playerY += velocityY;
	velocityX = max(velocityX-20, 0);
	velocityY = max(velocityY-20, 0);
}

bool checkColisions(){
	int mx;
	int my;
	int mapPos;
	int dof;
	int distanceTotal;
	float rayX;
	float rayY;
	float rayAngle;
	float x0;
	float y0;

	rayAngle = playerAngle;
	rayAngle = fixAngle(rayAngle);

		int verticalMapText = 0;
		int horizontalMapText= 0;
		dof = 0;
		x0 = 0;
		y0 = 0;
		float distanceH = 10000000000;
		float horizontalX = playerX;
		float horizontalY = playerY;
		//check horizontal lines
		float aTan = -1/tan(rayAngle); 
		if(rayAngle > PI){   //looking down
			rayY = (((int)playerY>>6)<<6) - 0.0001;
			rayX = (playerY - rayY) * aTan + playerX;
			y0 -= 64;
			x0 -= y0 * aTan;
		}
		if(rayAngle < PI){   //looking up
			rayY = (((int)playerY>>6)<<6) + 64;
			rayX = (playerY - rayY) * aTan + playerX;
			y0 = 64;
			x0 -= y0 * aTan;
		}
		// looking straight left or right
		if(rayAngle == 0 || rayAngle == PI){
			rayX = playerX;
			rayY = playerY;
			dof = 8;
		}
		while(dof < 8){
			mx = (int)(rayX)>>6;
			my = (int)(rayY)>>6;
			mapPos = my * mapX + mx;
			if(mapPos > 0 && mapPos< mapX*mapY && mapWalls[mapPos] >0){ // hit wall
				horizontalX = rayX;
				horizontalY = rayY;
				distanceH = distance(playerX, playerY, horizontalX, horizontalY, rayAngle);
				dof = 8;
			}
			else{
				rayX += x0;
				rayY += y0;
				dof++;
			}
		}
		dof = 0;
		float distanceV = 10000000000;
		float verticalX = playerX;
		float verticalY = playerY;
		x0 = 0;
		y0 = 0;


		//check vertical lines
		float nTan = -tan(rayAngle); 
		if(rayAngle > PI2 && rayAngle <PI3){   //looking down
			rayX = (((int)playerX>>6)<<6) - 0.0001;
			rayY = (playerX - rayX) * nTan + playerY;
			x0 -= 64;
			y0 -= x0 * nTan;
		}
		if(rayAngle < PI2 || rayAngle > PI3){   //looking up
			rayX = (((int)playerX>>6)<<6) + 64;
			rayY = (playerX - rayX) * nTan + playerY;
			x0 = 64;
			y0 -= x0 * nTan;
		}
		// looking straight left or right
		if(rayAngle == 0 || rayAngle == PI){
			rayX = playerX;
			rayY = playerY;
			dof = 8;
		}
		while(dof < 8){
			mx = (int)(rayX)>>6;
			my = (int)(rayY)>>6;
			mapPos = my * mapX + mx;
			if(mapPos > 0 && mapPos< mapX*mapY && mapWalls[mapPos] >0){ // hit wall
				verticalX = rayX;
				verticalY = rayY;
				distanceV = distance(playerX, playerY, verticalX, verticalY, rayAngle);
				dof = 8;
				verticalMapText = mapWalls[mapPos]-1;
			}
			else{
				rayX += x0;
				rayY += y0;
				dof++;
			}
		}
		if(distanceV < distanceH){
			horizontalMapText = verticalMapText;
			rayX = verticalX;
			rayY = verticalY;
			// distanceTotal = distanceV;
			// SDL_SetRenderDrawColor(gRenderer, 240, 0, 0, 0xFF);
		}
		if(distanceH < distanceV){
			rayX = horizontalX;
			rayY = horizontalY;
			// distanceTotal = distanceH;
			// SDL_SetRenderDrawColor(gRenderer, 100, 0, 0, 0xFF);
		}
		SDL_RenderDrawLine(gRenderer, playerX+4, playerY+4, rayX, rayY);
}

bool events(SDL_Event *e)
{
	// Handle events on queue
	while (SDL_PollEvent(e) != 0)
	{
		// User requests quit
		if (e->type == SDL_QUIT)
		{
			return true;
		}
		 if (e->type == SDL_MOUSEMOTION)
        {
			float lastPos = mouseX;
            mouseX = (float)e->motion.x - CTR_X;
            // mouseY = (float)e->motion.y;
            if(mouseX < lastPos){
				playerAngle -= 0.02;
				if(playerAngle < 0)
					playerAngle += 2*PI;
				playerDeltaX = cos(playerAngle)*5;
				playerDeltaY = sin(playerAngle)*5;
			}
			else if(mouseX > lastPos){
				playerAngle += 0.02;
				if(playerAngle > 2*PI)
					playerAngle -= 2*PI;
				playerDeltaX = cos(playerAngle)*5;
				playerDeltaY = sin(playerAngle)*5;
			}
			if(mouseX > SCREEN_WIDTH || mouseX < 0)
				SDL_WarpMouseInWindow(gWindow, CTR_X, CTR_Y);

        }
		const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
		int xOffset = 0;
		int yOffset = 0;
		if(playerDeltaX < 0)
			xOffset -= 20;
		else
			xOffset = 20;
		if(playerDeltaY < 0)
			yOffset -= 20;
		else
			yOffset = 20;
		int iPlayerX = playerX / 64;
		int iPlayerXAdd = (playerX + xOffset)/64;
		int iPlayerXSub = (playerX - xOffset)/64;

		int iPlayerY = playerY / 64;
		int iPlayerYAdd = (playerY + yOffset)/64;
		int iPlayerYSub = (playerY - yOffset)/64;
		if( currentKeyStates[ SDL_SCANCODE_W ] )
			{
				if(mapWalls[iPlayerY * mapX + iPlayerXAdd] == 0){
					playerX += playerDeltaX; //* 0.2 * fps;
					// velocityX += min(playerDeltaX, MAX_VELOCITY);
					// movePlayer();
				}
				if(mapWalls[iPlayerYAdd * mapX + iPlayerX] == 0){
					
					// velocityY += min(playerDeltaY, MAX_VELOCITY);
					// movePlayer();
					playerY += playerDeltaY; //* 0.2 * fps;
				}
			}
		else if( currentKeyStates[ SDL_SCANCODE_S ] )
			{
				if(mapWalls[iPlayerY * mapX + iPlayerXSub] == 0){
					// playerX -= playerDeltaX; //* 0.2 * fps;
					velocityX -= min(playerDeltaX, MAX_VELOCITY);
					movePlayer();
				}
				if(mapWalls[iPlayerYSub * mapX + iPlayerX] == 0){
					// playerY -= playerDeltaY; //* 0.2 * fps;
					velocityY -= min(playerDeltaY, MAX_VELOCITY);
					movePlayer();
				}
			}
		else if( currentKeyStates[ SDL_SCANCODE_A ] )
			{
				playerAngle -= 0.1;
				if(playerAngle < 0)
					playerAngle += 2*PI;
				playerDeltaX = cos(playerAngle)*5;
				playerDeltaY = sin(playerAngle)*5;
			}
		else if( currentKeyStates[ SDL_SCANCODE_D] )
			{
				playerAngle += 0.1;
				if(playerAngle > 2*PI)
					playerAngle -= 2*PI;
				playerDeltaX = cos(playerAngle)*5;
				playerDeltaY = sin(playerAngle)*5;
			}
		else if(currentKeyStates[SDL_SCANCODE_E]){
				if(playerDeltaX < 0)
					xOffset -= 25;
				else
					xOffset = 25;
				if(playerDeltaY < 0)
					yOffset -= 25;
				else
					yOffset = 25;
				iPlayerX = playerX / 64;
				iPlayerXAdd = (playerX + xOffset)/64;
				iPlayerY = playerY / 64;
				iPlayerYAdd = (playerY + yOffset)/64;
				if(mapWalls[iPlayerYAdd*mapX + iPlayerXAdd] == 4)
					mapWalls[iPlayerYAdd*mapX + iPlayerXAdd] = 0;
		}
	}
	return false;
}


void drawPlayer(){
	// Drawing player
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 0xFF);
	SDL_Rect playerRect = {playerX, playerY, 8, 8};
	SDL_RenderFillRect(gRenderer, &playerRect);

	SDL_SetRenderDrawColor(gRenderer, 255,255,255,0);
	SDL_RenderDrawLine(gRenderer, playerX+4, playerY+4, playerX+playerDeltaX*5+4, playerY+ playerDeltaY*5+4);
}

void drawMap2D(){
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

float distance(float ax, float ay, float bx, float by, float angle){
	return sqrt((bx-ax) * (bx-ax) + (by-ay) * (by-ay));

}

float fixAngle(float angle){
	if(angle < 0){
		angle += 2*PI;
	}
	if(angle > 2*PI){
		angle -= 2*PI;
	}
	return angle;
}

void drawRays(){
	int mx;
	int my;
	int mapPos;
	int dof;
	int distanceTotal;
	float rayX;
	float rayY;
	float rayAngle;
	float x0;
	float y0;

	rayAngle = playerAngle - DEGREE*30;
	// if(rayAngle < 0){
	// 	rayAngle += 2*PI;
	// }
	// if(rayAngle > 2*PI){
	// 	rayAngle -= 2*PI;
	// }
	rayAngle = fixAngle(rayAngle);

	for(int i = 0; i < 60;i++){
		int verticalMapText = 0;
		int horizontalMapText= 0;
		dof = 0;
		x0 = 0;
		y0 = 0;
		float distanceH = 10000000000;
		float horizontalX = playerX;
		float horizontalY = playerY;
		//check horizontal lines
		float aTan = -1/tan(rayAngle); 
		if(rayAngle > PI){   //looking down
			rayY = (((int)playerY>>6)<<6) - 0.0001;
			rayX = (playerY - rayY) * aTan + playerX;
			y0 -= 64;
			x0 -= y0 * aTan;
		}
		if(rayAngle < PI){   //looking up
			rayY = (((int)playerY>>6)<<6) + 64;
			rayX = (playerY - rayY) * aTan + playerX;
			y0 = 64;
			x0 -= y0 * aTan;
		}
		// looking straight left or right
		if(rayAngle == 0 || rayAngle == PI){
			rayX = playerX;
			rayY = playerY;
			dof = 8;
		}
		while(dof < 8){
			mx = (int)(rayX)>>6;
			my = (int)(rayY)>>6;
			mapPos = my * mapX + mx;
			if(mapPos > 0 && mapPos< mapX*mapY && mapWalls[mapPos] >0){ // hit wall
				horizontalX = rayX;
				horizontalY = rayY;
				distanceH = distance(playerX, playerY, horizontalX, horizontalY, rayAngle);
				dof = 8;
				horizontalMapText = mapWalls[mapPos]-1;
			}
			else{
				rayX += x0;
				rayY += y0;
				dof++;
			}
		}
		// SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 0xFF);
		// SDL_RenderDrawLine(gRenderer, playerX, playerY , rayX, rayY);

		dof = 0;
		float distanceV = 10000000000;
		float verticalX = playerX;
		float verticalY = playerY;
		x0 = 0;
		y0 = 0;
		//check vertical lines
		float nTan = -tan(rayAngle); 
		if(rayAngle > PI2 && rayAngle <PI3){   //looking down
			rayX = (((int)playerX>>6)<<6) - 0.0001;
			rayY = (playerX - rayX) * nTan + playerY;
			x0 -= 64;
			y0 -= x0 * nTan;
		}
		if(rayAngle < PI2 || rayAngle > PI3){   //looking up
			rayX = (((int)playerX>>6)<<6) + 64;
			rayY = (playerX - rayX) * nTan + playerY;
			x0 = 64;
			y0 -= x0 * nTan;
		}
		// looking straight left or right
		if(rayAngle == 0 || rayAngle == PI){
			rayX = playerX;
			rayY = playerY;
			dof = 8;
		}
		while(dof < 8){
			mx = (int)(rayX)>>6;
			my = (int)(rayY)>>6;
			mapPos = my * mapX + mx;
			if(mapPos > 0 && mapPos< mapX*mapY && mapWalls[mapPos] >0){ // hit wall
				verticalX = rayX;
				verticalY = rayY;
				distanceV = distance(playerX, playerY, verticalX, verticalY, rayAngle);
				dof = 8;
				verticalMapText = mapWalls[mapPos]-1;
			}
			else{
				rayX += x0;
				rayY += y0;
				dof++;
			}
		}
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
		// SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 0xFF);
		 SDL_RenderDrawLine(gRenderer, playerX+4, playerY+4, rayX, rayY);

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
		// SDL_Rect wall = {i*8+530, lineOffset, 8,lineHeight+lineOffset};
		// SDL_RenderFillRect(gRenderer, &wall);
		// SDL_RenderDrawLine(gRenderer, i*8+530,lineOffset,i*8+530, lineHeight+lineOffset);
		// for(int j = 0; j < 8;j++)
		// 	SDL_RenderDrawLine(gRenderer, i*8+530+j,lineOffset,i*8+530+j, lineHeight+lineOffset);
		float textureY = textureYOffset * textureYStep + horizontalMapText*32;
		float textureX;
		if(shading == 1){
			textureX = (int)(rayX/2.0)%32;
			if(rayAngle < PI){
				textureX = 31-textureX;
			}
		}
		else{
			textureX = (int)(rayY/2.0)%32;
			if(rayAngle > PI2 && rayAngle < PI3){
				textureX = 31-textureX;
			}
		}
		for(int j = 0; j < lineHeight;j++)
		{
			float color = textures[(int)(textureY)*32 + (int)(textureX)]*shading;
			SDL_SetRenderDrawColor(gRenderer, color*255, color*255,color*255, 255);
			for(int line = 0; line < 8;line++)
				SDL_RenderDrawPoint(gRenderer, i*8+530+line,j+lineOffset);
			textureY += textureYStep;
		}
		//printf("%f\n", rayAngle);

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
			// SDL_RenderDrawPoint(gRenderer, i*8+530,j);
			for(int line = 0; line < 8;line++){
				SDL_RenderDrawPoint(gRenderer, i*8+530+line,j);
			}
			
			//DRAW CEILING
			map = mapCeiling[(int)(textureY/32.0)*mapX+(int)(textureX/32.0)]*32*32;
			color = textures[((int)(textureY)&31)*32 + ((int)(textureX)&31)+map] *0.7;
			SDL_SetRenderDrawColor(gRenderer, color/2.0*255, color/1.2*255,color/2.0*255, 255);
			// SDL_RenderDrawPoint(gRenderer, i*8+530,j);
			for(int line = 0; line < 8;line++){
				SDL_RenderDrawPoint(gRenderer, i*8+530+line,320 - j);
			}

		}

		rayAngle += DEGREE;
		rayAngle = fixAngle(rayAngle);
	}
}


void display()
{
	// Clear screen
	SDL_SetRenderDrawColor(gRenderer, 105, 105, 105, 0);
	SDL_RenderClear(gRenderer);

	// frame2 = SDL_GetTicks64();
	// fps = frame2 - frame1;
	// frame1 = SDL_GetTicks64();
	//drawing mapWalls
	drawMap2D();

	//draw player
	drawPlayer();

	checkColisions();

	drawRays();
	// Update screen
	SDL_RenderPresent(gRenderer);
}

int main(int argc, char *args[])
{
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		bool quit = false;
		int frameCount = 0;
		SDL_Event e;

		// frame2 = SDL_GetTicks();
		// fps = frame2 - frame1;
		// frame1 = SDL_GetTicks();
		
		while (!quit)
		{
			quit = events(&e);
			display();
			//float avgFPS = frameCount / ( SDL_GetTicks64() / 1000.f );
			// frameCount++;
			// if(frameCount % 60 == 0)
			// printf("%f\n", avgFPS);
		}
	}

	close();

	return 0;
}