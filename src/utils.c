#include <math.h>
#include <string.h>
#include "../include/utils.h"

// Calculates distance between two points
float distance(float ax, float ay, float bx, float by){
	return (bx-ax) * (bx-ax) + (by-ay) * (by-ay);
}

// Calculates distance between two points relative to an angle
float distanceAngle(float angle, float ax, float ay, float bx, float by){
	return cos(angle)*(bx-ax)-sin(angle)*((by)-ay);
}

//FIxes angle, increments 2PI under 0 and vice versa if over 2PI
float fixAngle(float angle){
	if(angle < 0){
		angle += 2*PI;
	}
	if(angle > 2*PI){
		angle -= 2*PI;
	}
	return angle;
}

float simetricalAngle(float angle){
	if(angle == PI || angle == 0)
		return angle;
	return -angle;
}

float max(float x, float y){
	return x > y ? x : y;
}

float min(float x,float y){
	return x < y ? x : y;
}

//Timer functions
//-----------------------------------------------------------------------------------------------------

//Timer constructor
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
// --------------------------------------------------------------------------------------------------

//Texture constructor
LTexture* ltexture(SDL_Texture* texture, void*mPixels, int mPitch, int width, int height){
	LTexture* result = (LTexture*)malloc(sizeof(LTexture));
	LTexture x = {texture, mPixels, mPitch,width, height};
	*result = x;
	return result;
}

//Texture Destructor
void freeTexture(LTexture* texture){
	SDL_DestroyTexture(texture->texture);
	free(texture);
}

//Sets textures transparency color to given rgb values
void setTransparentColor(SDL_Window* gWindow,LTexture* text, Uint8 r, Uint8 g, Uint8 b){
		lockTexture(text);

		Uint32 format = SDL_GetWindowPixelFormat( gWindow );
		SDL_PixelFormat* mappingFormat = SDL_AllocFormat( format );
			// printf("%d\n", mappingFormat->Rmask);
		//Get pixel data
		Uint32* pixels = (Uint32*)text->mPixels;
		int pixelCount = (text->mPitch / 4 ) *text->height;

		//Map colors
		Uint32 colorKey = SDL_MapRGB( mappingFormat, r, g, b );
		Uint32 transparent = SDL_MapRGBA( mappingFormat, 0xFF, 0xFF, 0xFF, 0x00);	

		for(int i = 0; i < pixelCount;i++){
			if(pixels[i] == colorKey)
				pixels[i] = transparent;
		}

		//Unlock texture
		unlockTexture(text);

		//Free format
		SDL_FreeFormat( mappingFormat );
}

// Loads image from path and returns LTexture pointer
LTexture* loadFromFile(const char *path, SDL_Renderer* gRenderer,SDL_Window* gWindow){
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

//Renders texture, in positon x, y, from region of texture given by clip, stretched to variables width and height
void renderTexture(SDL_Renderer* gRenderer,LTexture* mtexture, int x, int y, SDL_Rect* clip, int width, int height){
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, mtexture->width, mtexture->height };
	if(clip != NULL){
		renderQuad.w = width;
        renderQuad.h = height;
	}
    SDL_RenderCopy( gRenderer, mtexture->texture, clip, &renderQuad );
}

//Analog function to renderTexture but for float coordinates
void renderTextureF(SDL_Renderer* gRenderer,LTexture* mtexture, float x, float y, SDL_Rect* clip, float width, float height){
    //Set rendering space and render to screen
    SDL_FRect renderQuad = { x, y, mtexture->width, mtexture->height };
	if(clip != NULL){
		renderQuad.w = width;
        renderQuad.h = height;
	}
    SDL_RenderCopyF( gRenderer, mtexture->texture, clip, &renderQuad );
}

// Locks texture for pixel manipulation
void lockTexture(LTexture* texture)
{

	//Texture is already locked
	if( texture->mPixels != NULL )
	{
		printf( "Texture is already locked!\n" );
	}
	//Lock texture
	else
	{
		if( SDL_LockTexture( texture->texture, NULL, &texture->mPixels, &texture->mPitch ) != 0 )
		{
			printf( "Unable to lock texture! %s\n", SDL_GetError() );
		}
	}

}

//Unlocks texture, to be used after pixel manipulation
void unlockTexture(LTexture* texture)
{

	//Texture is not locked
	if( texture->mPixels == NULL )
	{
		printf( "Texture is not locked!\n" );
	}
	//Unlock texture
	else
	{
		SDL_UnlockTexture( texture->texture );
		texture->mPixels = NULL;
		texture->mPitch = 0;
	}

}

bool is_perpendicular(float ax, float ay, float bx, float by){
	return (int)((ax * bx) + (ay * by)) == 0;
}

