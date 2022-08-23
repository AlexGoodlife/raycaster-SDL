
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "include\raycast.h"

const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 640;

#define CTR_X (SCREEN_WIDTH / 2)
#define CTR_Y (SCREEN_HEIGHT / 2)

LTexture* textures[4];
Sprite* Lsprites[7];
Sprite* SortedSprites[n_sprites];

Player* player;

float mouseX;
float mouseY;
float time = 10000;

float avgFPS;

// The window we'll be rendering to
SDL_Window *gWindow = NULL;

// The surface contained by the window
SDL_Surface *gScreenSurface = NULL;

//the renderer
SDL_Renderer *gRenderer = NULL;

bool init(){
	// Initialization flag
	bool success = true;

	//initialize player
	player = (Player*)malloc(sizeof(Player));
	player->x = 150;
	player->y = 300;
	player->angle = 0;
	player->deltaX = cos(player->angle);
	player->deltaY = sin(player->angle);

	BARREL_1 = spriteMapCons(1,0,true,STATIC_COLIDE,NULL,1);
	LAMP = spriteMapCons(2,0,true,DYNAMIC,NULL,1);
	PILLAR_1 = spriteMapCons(3,0,true,STATIC_COLIDE,NULL,1);
	PILLAR_2 = spriteMapCons(4,0,true,STATIC_COLIDE,NULL,1);
	PILLAR_3 = spriteMapCons(5,0,true,STATIC_COLIDE,NULL,1);
	ARMOUR_1 = spriteMapCons(6,0,true,STATIC_COLIDE,NULL,1);
	GUARD_1 = spriteMapCons(7,0,true,DYNAMIC,NULL, 8);

	// memcpy everything in Lsprites to SortedSprites, because we are copying pointers we can do this in init
	memcpy(SortedSprites, Lsprites, sizeof(Sprite*)*n_sprites);


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
	return success;
}

bool loadMedia(){
	//Loading success flag
	bool success = true;

	//Load wall textures
	textures[0] = loadFromFile("textures/greystone.png", gRenderer, gWindow);
	textures[1] = loadFromFile("textures/bluestone.png", gRenderer,gWindow);
	textures[2] = loadFromFile("textures/colorstone.png",gRenderer,gWindow);
	textures[3] = loadFromFile("textures/eagle.png",gRenderer, gWindow);

	//Load sprite textures
	BARREL_1->texture[0] = loadFromFile("sprites/barrel_2.png", gRenderer,gWindow);
	LAMP->texture[0] = loadFromFile("sprites/greenlight_2.png", gRenderer, gWindow);
	PILLAR_1->texture[0] = loadFromFile("sprites/pillar_3_t.png", gRenderer, gWindow);
	PILLAR_2->texture[0] = loadFromFile("sprites/pillar_3_t.png", gRenderer, gWindow);
	PILLAR_3->texture[0] = loadFromFile("sprites/pillar_3_t.png", gRenderer, gWindow);
	ARMOUR_1->texture[0] = loadFromFile("sprites/knight_t.png", gRenderer, gWindow);

	GUARD_1->texture[0] = loadFromFile("sprites/guard/guard_1_t.png", gRenderer, gWindow);
	GUARD_1->texture[1] = loadFromFile("sprites/guard/guard_2_t.png", gRenderer, gWindow);
	GUARD_1->texture[2] = loadFromFile("sprites/guard/guard_3_t.png", gRenderer, gWindow);
	GUARD_1->texture[3] = loadFromFile("sprites/guard/guard_4_t.png", gRenderer, gWindow);
	GUARD_1->texture[4] = loadFromFile("sprites/guard/guard_5_t.png", gRenderer, gWindow);
	GUARD_1->texture[5] = loadFromFile("sprites/guard/guard_6_t.png", gRenderer, gWindow);
	GUARD_1->texture[6] = loadFromFile("sprites/guard/guard_7_t.png", gRenderer, gWindow);
	GUARD_1->texture[7] = loadFromFile("sprites/guard/guard_8_t.png", gRenderer, gWindow);

	//Check if all textures were loaded properly
	for(int i = 0; i < n_textures; i++){
		if(textures[i] == NULL){
		success = false;
		}
	}

	//Check if all sprite textures were loaded properly
	for(int i = 0; i < n_sprites;i++){
		for(int j = 0; j < Lsprites[i]->n_texts;j++){
			if(Lsprites[i]->texture[j] == NULL)
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

bool events(SDL_Event *e){
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
			//mouse_movement(e);
        }
	}
	return false;
}

void playerMovement(){
	float oldTime = time;
	time = SDL_GetTicks();
	double frameTime = (time - oldTime);
	double rotSpeed = frameTime; //the constant value is in radians/second
	const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
    if( currentKeyStates[ SDL_SCANCODE_W ] )
        {
			if(checkColisions(gRenderer, player,0))
				movePlayer(gRenderer, player, FORWARD, rotSpeed);
        }
    if( currentKeyStates[ SDL_SCANCODE_S ] )
        {
			if(checkColisions(gRenderer, player,PI))
				movePlayer(gRenderer, player, BACKWARDS,rotSpeed);
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
			movePlayer(gRenderer, player, RIGHT,rotSpeed);
    }
	if( currentKeyStates[ SDL_SCANCODE_A] )
    {
		if(checkColisions(gRenderer, player,PI/2))
			movePlayer(gRenderer, player, LEFT, rotSpeed);
    }
    if(currentKeyStates[SDL_SCANCODE_F])
	{
		openDoor(player);
    }
}

void display(){
	// Clear screen
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear(gRenderer);

	drawFloors(gRenderer, SCREEN_HEIGHT, SCREEN_WIDTH);
	//draw rays and walls
	drawRays(gRenderer, player);

	drawSprites(gRenderer,player);

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

	// Point global pointers to NULL
	gRenderer = NULL;
	gWindow = NULL;

	// Destroy all textures
	for(int i = 0; i < n_textures;i++)
		freeTexture(textures[i]);

	// Free Player
	free(player);

	//Free both sprite arrays
	for(int i = 0; i < n_sprites;i++){
		freeSprite(Lsprites[i]);
		freeSprite(SortedSprites[i]);
	}

	// Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char *args[])
{
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
			int frames = 0;

			startTimer(&mTimer);
			while (!quit){
				quit = events(&e);
				playerMovement();
				avgFPS = getTimerTicks(&mTimer)/ 1000.0f;
				avgFPS = frames / (getTimerTicks(&mTimer) / 1000.f);
				if( avgFPS > 2000000 )
				{
				    avgFPS = 0;
				}
				// printf("%f\n", avgFPS);
				display();
				frames++;
			}
		}
	}
	close();
	return 0;
}
