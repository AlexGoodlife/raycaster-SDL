
#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "include\raycast.h"


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


void movePlayer(){
	playerX += velocityX;
	playerY += velocityY;
	velocityX = max(velocityX-20, 0);
	velocityY = max(velocityY-20, 0);
}


// bool events(SDL_Event *e)
// {
// 	// Handle events on queue
// 	while (SDL_PollEvent(e) != 0)
// 	{
// 		// User requests quit
// 		if (e->type == SDL_QUIT)
// 		{
// 			return true;
// 		}
// 		 if (e->type == SDL_MOUSEMOTION)
//         {
// 			float lastPos = mouseX;
//             mouseX = (float)e->motion.x - CTR_X;
//             // mouseY = (float)e->motion.y;
//             if(mouseX < lastPos){
// 				playerAngle -= 0.02;
// 				if(playerAngle < 0)
// 					playerAngle += 2*PI;
// 				playerDeltaX = cos(playerAngle)*5;
// 				playerDeltaY = sin(playerAngle)*5;
// 			}
// 			else if(mouseX > lastPos){
// 				playerAngle += 0.02;
// 				if(playerAngle > 2*PI)
// 					playerAngle -= 2*PI;
// 				playerDeltaX = cos(playerAngle)*5;
// 				playerDeltaY = sin(playerAngle)*5;
// 			}
// 			if(mouseX > SCREEN_WIDTH || mouseX < 0)
// 				SDL_WarpMouseInWindow(gWindow, CTR_X, CTR_Y);

//         }
// 		const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
// 		int xOffset = 0;
// 		int yOffset = 0;
// 		if(playerDeltaX < 0)
// 			xOffset -= 20;
// 		else
// 			xOffset = 20;
// 		if(playerDeltaY < 0)
// 			yOffset -= 20;
// 		else
// 			yOffset = 20;
// 		int iPlayerX = playerX / 64;
// 		int iPlayerXAdd = (playerX + xOffset)/64;
// 		int iPlayerXSub = (playerX - xOffset)/64;

// 		int iPlayerY = playerY / 64;
// 		int iPlayerYAdd = (playerY + yOffset)/64;
// 		int iPlayerYSub = (playerY - yOffset)/64;
// 		if( currentKeyStates[ SDL_SCANCODE_W ] )
// 			{
// 				if(mapWalls[iPlayerY * mapX + iPlayerXAdd] == 0){
// 					playerX += playerDeltaX; //* 0.2 * fps;
// 					// velocityX += min(playerDeltaX, MAX_VELOCITY);
// 					// movePlayer();
// 				}
// 				if(mapWalls[iPlayerYAdd * mapX + iPlayerX] == 0){
					
// 					// velocityY += min(playerDeltaY, MAX_VELOCITY);
// 					// movePlayer();
// 					playerY += playerDeltaY; //* 0.2 * fps;
// 				}
// 			}
// 		else if( currentKeyStates[ SDL_SCANCODE_S ] )
// 			{
// 				if(mapWalls[iPlayerY * mapX + iPlayerXSub] == 0){
// 					// playerX -= playerDeltaX; //* 0.2 * fps;
// 					velocityX -= min(playerDeltaX, MAX_VELOCITY);
// 					movePlayer();
// 				}
// 				if(mapWalls[iPlayerYSub * mapX + iPlayerX] == 0){
// 					// playerY -= playerDeltaY; //* 0.2 * fps;
// 					velocityY -= min(playerDeltaY, MAX_VELOCITY);
// 					movePlayer();
// 				}
// 			}
// 		else if( currentKeyStates[ SDL_SCANCODE_A ] )
// 			{
// 				playerAngle -= 0.1;
// 				if(playerAngle < 0)
// 					playerAngle += 2*PI;
// 				playerDeltaX = cos(playerAngle)*5;
// 				playerDeltaY = sin(playerAngle)*5;
// 			}
// 		else if( currentKeyStates[ SDL_SCANCODE_D] )
// 			{
// 				playerAngle += 0.1;
// 				if(playerAngle > 2*PI)
// 					playerAngle -= 2*PI;
// 				playerDeltaX = cos(playerAngle)*5;
// 				playerDeltaY = sin(playerAngle)*5;
// 			}
// 		else if(currentKeyStates[SDL_SCANCODE_E]){
// 				if(playerDeltaX < 0)
// 					xOffset -= 25;
// 				else
// 					xOffset = 25;
// 				if(playerDeltaY < 0)
// 					yOffset -= 25;
// 				else
// 					yOffset = 25;
// 				iPlayerX = playerX / 64;
// 				iPlayerXAdd = (playerX + xOffset)/64;
// 				iPlayerY = playerY / 64;
// 				iPlayerYAdd = (playerY + yOffset)/64;
// 				if(mapWalls[iPlayerYAdd*mapX + iPlayerXAdd] == 4)
// 					mapWalls[iPlayerYAdd*mapX + iPlayerXAdd] = 0;
// 		}
// 	}
// 	return false;
// }

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
		//  if (e->type == SDL_MOUSEMOTION)
        // {
		// 	float lastPos = mouseX;
        //     mouseX = (float)e->motion.x - CTR_X;
        //     // mouseY = (float)e->motion.y;
        //     if(mouseX < lastPos){
		// 		playerAngle -= 0.02;
		// 		if(playerAngle < 0)
		// 			playerAngle += 2*PI;
		// 		playerDeltaX = cos(playerAngle)*5;
		// 		playerDeltaY = sin(playerAngle)*5;
		// 	}
		// 	else if(mouseX > lastPos){
		// 		playerAngle += 0.02;
		// 		if(playerAngle > 2*PI)
		// 			playerAngle -= 2*PI;
		// 		playerDeltaX = cos(playerAngle)*5;
		// 		playerDeltaY = sin(playerAngle)*5;
		// 	}
		// 	if(mouseX > SCREEN_WIDTH || mouseX < 0)
		// 		SDL_WarpMouseInWindow(gWindow, CTR_X, CTR_Y);

        // }
		const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
		if( currentKeyStates[ SDL_SCANCODE_W ] )
			{
				if(smallestRay > 20){
					playerX += playerDeltaX; //* 0.2 * fps;
                    playerY += playerDeltaY;
				// 	// velocityX += min(playerDeltaX, MAX_VELOCITY);
				// 	// movePlayer();
				}
				// if(checkColisions(gRenderer, playerAngle, playerX, playerY,playerDeltaX, playerDeltaY, 0)){
					
				// 	// velocityY += min(playerDeltaY, MAX_VELOCITY);
				// 	// movePlayer();
					// playerY += playerDeltaY; //* 0.2 * fps;
				// }
			}
		else if( currentKeyStates[ SDL_SCANCODE_S ] )
			{
				if(checkColisions(gRenderer, playerAngle, playerX, playerY,playerDeltaX, playerDeltaY, PI)){
					playerX -= playerDeltaX; //* 0.2 * fps;
                    playerY -= playerDeltaY; //* 0.2 * fps;
				// 	// velocityX -= min(playerDeltaX, MAX_VELOCITY);
				// 	// movePlayer();
				}
				// if(checkColisions(gRenderer, playerAngle, playerX, playerY,playerDeltaX, playerDeltaY, PI)){
					// playerY -= playerDeltaY; //* 0.2 * fps;
				// 	// velocityY -= min(playerDeltaY, MAX_VELOCITY);
				// 	// movePlayer();
				// }
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
				int xOffset = 0;
            int yOffset = 0;
            if(playerDeltaX < 0)
                xOffset -= 25;
            else
                xOffset = 25;
            if(playerDeltaY < 0)
                yOffset -= 25;
            else
                yOffset = 25;
            // int iPlayerX = playerX / 64;
            int iPlayerXAdd = (playerX + xOffset)/64;
            // int iPlayerXSub = (playerX - xOffset)/64;

            // int iPlayerY = playerY / 64;
            int iPlayerYAdd = (playerY + yOffset)/64;
            // int iPlayerYSub = (playerY - yOffset)/64;
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

    // checkColisions(gRenderer, playerAngle, playerX, playerY,playerDeltaX, playerDeltaY, 0);
    // checkColisions(gRenderer, playerAngle, playerX, playerY,playerDeltaX, playerDeltaY, PI);
	//draw player
	drawPlayer();

    
	drawRays(gRenderer, playerAngle, playerX, playerY);
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
		// int frameCount = 0;
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