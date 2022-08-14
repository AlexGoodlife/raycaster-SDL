
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "include\raycast.h"

const int SCREEN_WIDTH = 1020;
const int SCREEN_HEIGHT = 540;

#define CTR_X (SCREEN_WIDTH / 2)
#define CTR_Y (SCREEN_HEIGHT / 2)

float playerX;
float playerY;
float playerDeltaX;
float playerDeltaY;
float playerAngle;
float mouseX;
float mouseY;

float avgFPS;


// The window we'll be rendering to
SDL_Window *gWindow = NULL;

// The surface contained by the window
SDL_Surface *gScreenSurface = NULL;

SDL_Renderer *gRenderer = NULL;


bool init(){
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
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
				else
				{
					SDL_SetRenderDrawColor(gRenderer, 105, 105, 105, 0);
					SDL_SetRelativeMouseMode(SDL_TRUE);
					//Get window surface
					gScreenSurface = SDL_GetWindowSurface( gWindow );
				}
			}
		}
	}
	return success;
}

bool loadMedia(){
	//Loading success flag
	bool success = true;

	//Load sprite texture
	textures[0] = loadFromFile("textures/checker_3.png", gRenderer);
	textures[1] = loadFromFile("textures/checker.png", gRenderer);
	textures[2] = loadFromFile("textures/blue_checker.png",gRenderer);
	for(int i = 0; i < n_textures; i++){
		if(textures[i] == NULL){
		success = false;
		}
	}	
	return success;
}

void close(){
	// Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gRenderer = NULL;
	gWindow = NULL;
	for(int i = 0; i < n_textures;i++)
		free(textures[i]);
	// free(gSpriteSheetTexture);

	// Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

bool events(SDL_Event *e, float timeStep){
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
				playerAngle -= 0.008;
				if(playerAngle < 0)
					playerAngle += 2*PI;
				playerDeltaX = cos(playerAngle)*5;
				playerDeltaY = sin(playerAngle)*5;
			}
			else if(mouseX > lastPos){
				playerAngle += 0.008;
				if(playerAngle > 2*PI)
					playerAngle -= 2*PI;
				playerDeltaX = cos(playerAngle)*5;
				playerDeltaY = sin(playerAngle)*5;
			}
			if(mouseX > SCREEN_WIDTH || mouseX < 0){
				SDL_WarpMouseInWindow(gWindow, CTR_X, CTR_Y);
                lastPos = CTR_X;
            }

        }
        else{
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
			int iPlayerX = (int)playerX >> 6; // bitshift right by 6 to divide by 64
			int iPlayerXAdd = (int)(playerX + xOffset) >> 6;
			int iPlayerXSub = (int)(playerX - xOffset) >> 6;

			int iPlayerY = (int)playerY >> 6;
			int iPlayerYAdd = (int)(playerY + yOffset) >> 6;
			int iPlayerYSub = (int)(playerY - yOffset) >> 6;
            if( currentKeyStates[ SDL_SCANCODE_W ] )
                {
					bool canMove = checkColisions(gRenderer, playerAngle, playerX, playerY,0);
                    if(mapWalls[iPlayerY * mapX + iPlayerXAdd] == 0 && canMove){
						playerX += playerDeltaX; //* timeStep; //*1/avgFPS;
					}
					if(mapWalls[iPlayerYAdd * mapX + iPlayerX] == 0 && canMove){
						playerY += playerDeltaY; //*timeStep; //* 1/avgFPS;
					}
                }
            else if( currentKeyStates[ SDL_SCANCODE_S ] )
                {
				bool canMove = checkColisions(gRenderer, playerAngle, playerX, playerY,PI);

                if(mapWalls[iPlayerY * mapX + iPlayerXSub] == 0 && canMove){
						playerX -= playerDeltaX; //*timeStep; //* 0.2 * fps;
				}
				if(mapWalls[iPlayerYSub * mapX + iPlayerX] == 0 && canMove){
						playerY -= playerDeltaY; //*timeStep; //* 0.2 * fps;
				}
                }
            else if( currentKeyStates[ SDL_SCANCODE_Q ] )
                {
                    playerAngle = fixAngle(playerAngle -0.1);
                    playerDeltaX = cos(playerAngle)*5;
                    playerDeltaY = sin(playerAngle)*5;
                }
            else if( currentKeyStates[ SDL_SCANCODE_E] )
                {
					playerAngle = fixAngle(playerAngle +0.1);
                    playerDeltaX = cos(playerAngle)*5;
                    playerDeltaY = sin(playerAngle)*5;
                }
			  else if( currentKeyStates[ SDL_SCANCODE_D] )
                {
				int xOffset = 0;
				int yOffset = 0;
				float playerDeltaX_temp = cos(fixAngle(playerAngle + PI/2))*5;
                float playerDeltaY_temp = sin(fixAngle(playerAngle+ PI/2))*5;
				if(playerDeltaX_temp < 0)
					xOffset -= 20;
				else
					xOffset = 20;
				if(playerDeltaY_temp < 0)
					yOffset -= 20;
				else
					yOffset = 20;
				int iPlayerX = (int)playerX >> 6; // bitshift right by 6 to divide by 64
				int iPlayerXAdd = (int)(playerX + xOffset) >> 6;

				int iPlayerY = (int)playerY >> 6;
				int iPlayerYAdd = (int)(playerY + yOffset) >> 6;
				bool canMove = checkColisions(gRenderer, playerAngle, playerX, playerY,PI/2);
                    if(mapWalls[iPlayerY * mapX + iPlayerXAdd] == 0 && canMove){
						playerX += playerDeltaX_temp/2; //* timeStep; //*1/avgFPS;
					}
					if(mapWalls[iPlayerYAdd * mapX + iPlayerX] == 0 && canMove){
						playerY += playerDeltaY_temp/2; //*timeStep; //* 1/avgFPS;
					}
                }
			else if( currentKeyStates[ SDL_SCANCODE_A] )
                {
				int xOffset = 0;
				int yOffset = 0;
				float playerDeltaX_temp = cos(fixAngle(playerAngle - PI/2))*5;
                float playerDeltaY_temp = sin(fixAngle(playerAngle - PI/2))*5;
				if(playerDeltaX_temp < 0)
					xOffset -= 20;
				else
					xOffset = 20;
				if(playerDeltaY_temp < 0)
					yOffset -= 20;
				else
					yOffset = 20;
				int iPlayerX = (int)playerX >> 6; // bitshift right by 6 to divide by 64
				int iPlayerXAdd = (int)(playerX + xOffset) >> 6;

				int iPlayerY = (int)playerY >> 6;
				int iPlayerYAdd = (int)(playerY + yOffset) >> 6;
				bool canMove = checkColisions(gRenderer, playerAngle, playerX, playerY,-PI/2);
                    if(mapWalls[iPlayerY * mapX + iPlayerXAdd] == 0 && canMove){
						playerX += playerDeltaX_temp/2;// * timeStep; //*1/avgFPS;
					}
					if(mapWalls[iPlayerYAdd * mapX + iPlayerX] == 0 && canMove){
						playerY += playerDeltaY_temp/2;// *timeStep; //* 1/avgFPS;
					}
                }
            else if(currentKeyStates[SDL_SCANCODE_F]){
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
                int iPlayerXAdd = (int)(playerX + xOffset) >> 6; // bitshift right to divide by 64

                int iPlayerYAdd = (int)(playerY + yOffset) >> 6;

                if(mapWalls[iPlayerYAdd*mapX + iPlayerXAdd] == 4)
                    mapWalls[iPlayerYAdd*mapX + iPlayerXAdd] = 0;
            }
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

	//drawing mapWalls
	drawMap2D();

	//draw player
	drawPlayer();

	drawRays(gRenderer, playerAngle, playerX, playerY);

	// Update screen
	SDL_RenderPresent(gRenderer);
}

int main(int argc, char *args[])
{
	printf("%d\n", n_textures);
	if (!init()){
		printf("Failed to initialize!\n");
	}
	else
	{
		if(!loadMedia()){
			printf("FAILED TO LOAD MEDIA!\n");
		}
		else{
			bool quit = false;
			SDL_Event e;
			Timer mTimer = timer(0,0,false,false);
			Timer stepTimer = timer(0,0,false,false);
			int frames = 0;
			float timeStep;

			// startTimer(&mTimer);
			while (!quit){
				//Calculate time step
				timeStep = getTimerTicks(&stepTimer) *0.2;
				quit = events(&e, timeStep);
				avgFPS = getTimerTicks(&mTimer)/ 1000.0f;
				avgFPS = frames / (getTimerTicks(&mTimer) / 1000.f);
				if( avgFPS > 2000000 )
				{
				    avgFPS = 0;
				}
				// printf("%f\n", avgFPS);
				startTimer(&stepTimer);
				display();
				frames++;
			}
		}
	}
	close();
	return 0;
}