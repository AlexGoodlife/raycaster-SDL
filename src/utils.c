#include <math.h>

#include "../include/utils.h"


float distance(float ax, float ay, float bx, float by){
	return (bx-ax) * (bx-ax) + (by-ay) * (by-ay);
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

float max(float x, float y){
	return x > y ? x : y;
}

float min(float x,float y){
	return x < y ? x : y;
}

//Timer functions

Timer timer(Uint32 startTicks, Uint32 stopTicks, bool paused, bool started){
	Timer result = {startTicks, stopTicks, paused, started};
	return result;
}

void startTimer(Timer* mTimer){
	mTimer->started = true;
	mTimer->paused = false;

	mTimer->startTicks = SDL_GetTicks();
	mTimer->stopTicks = 0;
}

void stopTimer(Timer *mTimer){
	mTimer->started = false;
	mTimer->paused = false;

	mTimer->startTicks = 0;
	mTimer->stopTicks = 0;
}

void pauseTimer(Timer *mTimer){
	if(!mTimer->paused && mTimer->started){
		mTimer->stopTicks = SDL_GetTicks() - mTimer->startTicks;
		mTimer->startTicks = 0;
	}
}

void unpauseTimer(Timer *mTimer){
	if(mTimer->paused && mTimer->started){
		mTimer->paused = false;

		mTimer->startTicks = SDL_GetTicks() - mTimer->stopTicks;

		mTimer->stopTicks = 0;
	}
}

Uint32 getTimerTicks(Timer *mTimer){
	Uint32 time = 0;
	if(mTimer->started){
		if(mTimer->paused)
			time = mTimer->stopTicks;
		else
			time = SDL_GetTicks() - mTimer->startTicks;
	}
	return time;
}


// Texture funcs

LTexture* loadFromFile(const char *path, SDL_Renderer* gRenderer){
	SDL_Texture* newTexture = NULL;
	LTexture* result = malloc(sizeof(LTexture));

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError() );
    }
    else
    {
		//Color key image
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError() );
        }
		else{
			result->width = loadedSurface->w;
			result->height = loadedSurface->h;
		}

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }
	result->texture = newTexture;
	//SDL_DestroyTexture(newTexture);
    return result;
}

void renderTexture(SDL_Renderer* gRenderer,LTexture* mtexture, int x, int y, SDL_Rect* clip, int width, int height){
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, mtexture->width, mtexture->height };
	if(clip != NULL){
		renderQuad.w = width;
        renderQuad.h = height;
	}
    SDL_RenderCopy( gRenderer, mtexture->texture, clip, &renderQuad );
}

