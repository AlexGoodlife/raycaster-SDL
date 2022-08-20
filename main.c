
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "include\raycast.h"

const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 640;

#define CTR_X (SCREEN_WIDTH / 2)
#define CTR_Y (SCREEN_HEIGHT / 2)

LTexture* textures[3];
Sprite* Lsprites[6];

Player* player;

float mouseX;
float mouseY;
float time = 0;

float avgFPS;

// The window we'll be rendering to
SDL_Window *gWindow = NULL;

// The surface contained by the window
SDL_Surface *gScreenSurface = NULL;

//the renderer
SDL_Renderer *gRenderer = NULL;

FILE *f;

bool init(){
	// Initialization flag
	bool success = true;

	//initialize player
	player = (Player*)malloc(sizeof(Player));
	player->x = 150;
	player->y = 400;
	player->angle = 0;
	player->deltaX = cos(player->angle)*5;
	player->deltaY = sin(player->angle)*5;

	Lsprites[0] = sprite(1.5*64, 5*64,-20,0,true,1,NULL);
	Lsprites[1] = sprite(3.5*64, 5*64,-20,0,true,1,NULL);
	Lsprites[2] = sprite(2.5*64, 5*64,-20,0,true,1,NULL);
	Lsprites[3] = sprite(2.5*64, 4.6*64,-20,0,true,1,NULL);
	Lsprites[4] = sprite(2.5*64, 5.4*64,-20,0,true,1,NULL);
	Lsprites[5] = sprite(3.5*64, 6.5*64,-20,0,true,1,NULL);


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
				SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
				else
				{
					SDL_SetRenderDrawColor(gRenderer, 105, 105, 105, 0xFF);
					SDL_SetRelativeMouseMode(SDL_TRUE);
					//Get window surface
					gScreenSurface = SDL_GetWindowSurface( gWindow );
				}
			}
		}
	}
	Uint32 format = SDL_GetWindowPixelFormat( gWindow );
	SDL_PixelFormat* mappingFormat = SDL_AllocFormat( format );
	printf("%d\n",mappingFormat->Amask );
	return success;
}

bool loadMedia(){
	//Loading success flag
	bool success = true;

	//Load sprite texture
	textures[0] = loadFromFile("textures/greystone.png", gRenderer, gWindow);
	textures[1] = loadFromFile("textures/bluestone.png", gRenderer,gWindow);
	textures[2] = loadFromFile("textures/colorstone.png",gRenderer,gWindow);

	Lsprites[0]->texture = loadFromFile("sprites/barrel_2.png", gRenderer,gWindow);
	Lsprites[1]->texture = loadFromFile("sprites/greenlight_2.png", gRenderer, gWindow);
	Lsprites[2]->texture = loadFromFile("sprites/pillar_3_t.png", gRenderer, gWindow);
	Lsprites[3]->texture = loadFromFile("sprites/pillar_3_t.png", gRenderer, gWindow);
	Lsprites[4]->texture = loadFromFile("sprites/pillar_3_t.png", gRenderer, gWindow);
	Lsprites[5]->texture = loadFromFile("sprites/knight_t.png", gRenderer, gWindow);
	for(int i = 0; i < n_textures; i++){
		if(textures[i] == NULL){
		success = false;
		}
	}	
	return success;
}

void mouse_movement(SDL_Event *e){
	float lastPos = mouseX;
    mouseX = (float)e->motion.x - CTR_X;
	// mouseY = (float)e->motion.y;
	if(mouseX < lastPos){
		player->angle -= 0.008;
	if(player->angle < 0)
		player->angle += 2*PI;
	player->deltaX = cos(player->angle)*5;
	player->deltaY = sin(player->angle)*5;
	}
	else if(mouseX > lastPos){
		player->angle += 0.008;
		if(player->angle > 2*PI)
			player->angle -= 2*PI;
		player->deltaX = cos(player->angle)*5;
		player->deltaY = sin(player->angle)*5;
	}
	if(mouseX > SCREEN_WIDTH || mouseX < 0){
		SDL_WarpMouseInWindow(gWindow, CTR_X, CTR_Y);
        lastPos = CTR_X;
    }
}

bool events(SDL_Event *e,Timer *stepTimer, float timeStep){
	// Handle events on queue
	while (SDL_PollEvent(e) != 0)
	{
		float oldTime = time;
		time = SDL_GetTicks();
		double frameTime = (time - oldTime) / 1000.0;
		double rotSpeed = frameTime * 2.0; //the constant value is in radians/second
		// User requests quit
		if (e->type == SDL_QUIT)
		{
			return true;
		}

        if (e->type == SDL_MOUSEMOTION)
        {	
			//mouse_movement(e);
        }
        else{
            const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
            if( currentKeyStates[ SDL_SCANCODE_W ] )
            {
				if(checkColisions(gRenderer, player,0))
					movePlayer(gRenderer, player, FORWARD);
            }
            if( currentKeyStates[ SDL_SCANCODE_S ] )
            {
				if(checkColisions(gRenderer, player,PI))
					movePlayer(gRenderer, player, BACKWARDS);
            }
            if( currentKeyStates[ SDL_SCANCODE_Q ] )
			{
				playerLook(player, LEFT, rotSpeed);
            }
            if( currentKeyStates[ SDL_SCANCODE_E] )
            {
				playerLook(player, RIGHT, rotSpeed);
            }
			if( currentKeyStates[ SDL_SCANCODE_D] )
            {
				if(checkColisions(gRenderer, player,-PI/2))
					movePlayer(gRenderer, player, RIGHT);
            }
			if( currentKeyStates[ SDL_SCANCODE_A] )
            {
				if(checkColisions(gRenderer, player,PI/2))
					movePlayer(gRenderer, player, LEFT);
            }
            if(currentKeyStates[SDL_SCANCODE_F])
			{
				openDoor(player);
            }
        }
	}
	return false;
}


void display(){
	// Clear screen
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear(gRenderer);

	drawFloors(gRenderer, SCREEN_HEIGHT, SCREEN_WIDTH);
	//draw rays and walls
	drawRays(gRenderer, player);

	drawSprites(gRenderer, gWindow, player);

	// //drawing mapWalls
	// drawMap2D(gRenderer);

	// //draw player
	// drawPlayer(gRenderer, player);

	// Update screen
	SDL_RenderPresent(gRenderer);
}

void close(){
	// Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gRenderer = NULL;
	gWindow = NULL;
	for(int i = 0; i < n_textures;i++)
		free(textures[i]);
	free(player);
	free(Lsprites[0]);
	// Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char *args[])
{
	f = fopen("log.txt", "w");
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

			startTimer(&mTimer);
			startTimer(&stepTimer);
			while (!quit){
				//Calculate time step
				timeStep = getTimerTicks(&stepTimer) *0.2;
				quit = events(&e, &stepTimer, timeStep);
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
