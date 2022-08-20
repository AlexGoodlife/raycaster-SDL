#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "../include/raycast.h"

int depth[480];

void drawRays(SDL_Renderer *gRenderer, Player *player){
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

	float rayAngle = fixAngle(player->angle + DEGREE*30);

	for(int i = 0; i < 480;i++){
		verticalMapText = 0;
		horizontalMapText= 0;

		horizontalX = player->x;
		horizontalY = player->y;

		verticalX = player->x;
		verticalY = player->y;

		distanceV = renderVerticalRays(player, rayAngle, &verticalX, &verticalY, &rayX, &rayY, &verticalMapText);
		distanceH = renderHorizontalRays(player, rayAngle, &horizontalX, &horizontalY, &rayX, &rayY, &horizontalMapText);

		float shading = 1;
		if(distanceV < distanceH){
			horizontalMapText = verticalMapText;
			shading = 0.5;
			rayX = verticalX;
			rayY = verticalY;
			distanceTotal = (distanceV);
		}
		if(distanceH < distanceV){
			rayX = horizontalX;
			rayY = horizontalY;
			distanceTotal = (distanceH);
		}
		depth[i] = distanceTotal;
		// SDL_RenderDrawLine(gRenderer, player->x+4, player->y+4, rayX, rayY);

		//Draw 3D
		draw3D(gRenderer, player, rayAngle, distanceTotal, horizontalMapText,shading, rayX, rayY,i);
	
		rayAngle = fixAngle(rayAngle - DEGREE/8);
	}
}
float renderHorizontalRays(Player *player, float rayAngle, float *horizontalX, float *horizontalY, float *rayX, float *rayY, int *horizontalMapText){
	int dof = 0;
	float x0 = 0;
	float y0 = 0;
	int mx;
	int my;
	float distanceH = 10000000000;
	int mapPos;


	float Tan = 1/tan(rayAngle); 

	if(sin(rayAngle)> 0.001){//looking up 
		(*rayY)=(((int)player->y>>6)<<6) -0.0001; 
		(*rayX)=(player->y-(*rayY))*Tan+player->x; 
		y0=-64; 
		x0=-y0*Tan;
	}
	else if(sin((rayAngle))<-0.001){//looking down
		(*rayY)=(((int)player->y>>6)<<6)+64;      
		(*rayX)=(player->y-(*rayY))*Tan+player->x;
		y0= 64; 
		x0=-y0*Tan;
	}

	else{ 
		(*rayX)=player->x;
		(*rayY)=player->y; 
		dof=8;
	} 

	while(dof < 8){
		mx = (int)(*rayX)>>6;
		my = (int)(*rayY)>>6;
		mapPos = my * mapX + mx;
		if(mapPos > 0 && mapPos< mapX*mapY && mapWalls[mapPos] >0){ // hit wall
			*horizontalX = *rayX;
			*horizontalY = *rayY;
			distanceH = distanceAngle(rayAngle, player->x, player->y, *rayX, *rayY);
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

float renderVerticalRays(Player *player, float rayAngle, float *verticalX, float *verticalY, float *rayX, float *rayY, int *verticalMapText){
	int dof = 0;
	float x0 = 0;
	float y0 = 0;
	int mx;
	int my;
	float distanceV = 10000000000;
	int mapPos;

	//check vertical lines
	float Tan = tan(rayAngle); 

    if(cos(rayAngle)> 0.001) //looking left
	{ 
		(*rayX)=(((int)player->x>>6)<<6)+64;      
		(*rayY)=(player->x-(*rayX))*Tan+player->y; 
		x0= 64; 
		y0=-x0*Tan;
	}
	else if(cos(rayAngle)<-0.001){  //looking right
		(*rayX)=(((int)player->x>>6)<<6) -0.0001; 
		(*rayY)=(player->x-(*rayX))*Tan+player->y; 
		x0=-64; 
		y0=-x0*Tan;
	}
	else{ 
		(*rayX)=player->x; 
		(*rayY)=player->y; 
		dof=8;
	}
	while(dof < 8){
		mx = (int)(*rayX)>>6;
		my = (int)(*rayY)>>6;
		mapPos = my * mapX + mx;
		if(mapPos > 0 && mapPos< mapX*mapY && mapWalls[mapPos] >0){ // hit wall
			*verticalX = *rayX;
			*verticalY = *rayY;
			distanceV = distanceAngle(rayAngle, player->x, player->y, *rayX, *rayY);
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

void draw3D(SDL_Renderer *gRenderer, Player *player, float rayAngle, int distanceTotal, int horizontalMapText, float shading, float rayX, float rayY, int i){
	//Draw 3D
	float cAngle = player->angle - rayAngle;
	cAngle = fixAngle(cAngle);

	distanceTotal = distanceTotal* cos(cAngle);
	float lineHeight = (blockSize*640)/distanceTotal;
		
	int lineOffset = 320-(lineHeight/2);

	float textureX;
	if(shading == 1){
		SDL_SetTextureColorMod(textures[horizontalMapText]->texture, 255, 255, 255);
		textureX = (int)((rayX))%64;
		if(rayAngle > PI){
			textureX = 63-textureX;
		}
	}
	else{
		SDL_SetTextureColorMod(textures[horizontalMapText]->texture, 100, 100, 100);
		textureX = (int)((rayY))%64;
		if(rayAngle > PI2 && rayAngle < PI3){
			textureX = 63-textureX;
		}
	}

	// draw textured walls
	SDL_Rect wall = {textureX, 0, 1,64};
	renderTexture(gRenderer,textures[horizontalMapText], i*2,lineOffset,&wall, 2, lineHeight);


	// //floors SDL_RECT
	// for(int j = lineOffset + lineHeight;j < 640;j++){
	// 	SDL_Rect floor = {i*8, j, 8,1};
	// 	SDL_Rect ceil = {i*8, 640-j, 8, 1};
		
	// 	SDL_SetRenderDrawColor(gRenderer,127,127,127,255);
	// 	SDL_RenderFillRect(gRenderer, &floor);
	// 	SDL_SetRenderDrawColor(gRenderer,50,50,50,0);
	// 	SDL_RenderFillRect(gRenderer, &ceil);
	// }

	//  TEXTUREDFLOOR ATTEMPT

	// for(int j = lineOffset + lineHeight;j < 540;j++){
	// 	// printf("%d\n", j);
	// 	// printf("%f\n", rayAngle);
	// 	float deltaY = j - (320/2.0);
	// 	float degree = rayAngle;
	// 	float rayAngleFix = cos(fixAngle(player->angle - rayAngle));
	// 	textureX = (player->x/2 + (cos(degree)*158*32/deltaY/rayAngleFix));
	// 	float textureY = (player->y/2) - (sin(degree)*158*32/deltaY/rayAngleFix);
	// 	SDL_Rect floor = {(int)textureX%32,(int)textureY % 32, 1,32 };
	// 	int map = mapFloors[mapX+(int)(textureX/32.0)];
	// 	// printf("%d\n", map);
	// 	SDL_SetTextureColorMod(textures[0]->texture, 255, 255, 255);
	// 	renderTexture(gRenderer, textures[0], i*8,j,&floor,8,1);

	// }

}

bool checkColisions(SDL_Renderer *gRenderer, Player *player,float directionOffset){

	float rayX;
	float rayY;

	float rayAngle = player->angle + directionOffset;
	rayAngle = fixAngle(rayAngle);

	int verticalMapText = 0;
	int horizontalMapText= 0;

	float horizontalX = player->x;
	float horizontalY = player->y;

	float verticalX = player->x;
	float verticalY = player->y;

	float distanceH = renderHorizontalRays(player,rayAngle, &horizontalX, &horizontalY, &rayX, &rayY, &horizontalMapText);
	float distanceV = renderVerticalRays(player,rayAngle, &verticalX, &verticalY, &rayX, &rayY, &verticalMapText);

	if(distanceV < distanceH){
		horizontalMapText = verticalMapText;
		rayX = verticalX;
		rayY = verticalY;
	}
	if(distanceH < distanceV){
		rayX = horizontalX;
		rayY = horizontalY;
	}
	return sqrt(distance(player->x, player->y,rayX, rayY)) > 20;
}

void drawFloors(SDL_Renderer *gRenderer,int screenHeight, int screenWidth){
	SDL_SetRenderDrawColor(gRenderer, 116, 116,116,255);
	SDL_Rect floor = {0,screenHeight>>1, screenWidth, screenHeight >> 1};
	SDL_RenderFillRect(gRenderer, &floor);
	SDL_SetRenderDrawColor(gRenderer, 90,90,90,255);
	SDL_Rect ceil = {0,0, screenWidth, screenHeight >> 1};
	SDL_RenderFillRect(gRenderer, &ceil);
}

// SPRITE FUNCS

Sprite* sprite(float x, float y, float z, float playerDist,bool state, int type, LTexture **texture, int n_texts){
	Sprite * result = (Sprite*)malloc(sizeof(Sprite));
	result->texture = (LTexture**)malloc(sizeof(LTexture*)*n_texts);
	Sprite temp = {x, y, z, playerDist, state, type, result->texture, n_texts};
	*result = temp;
	return result;
}

int cmp_sprites(const void *x, const void *y){
	return -((*(Sprite**)x)->playerDist - (*(Sprite**)y)->playerDist);
}


void sortSprites(Player *player){
	for(int i = 0; i < n_sprites;i++){
		Lsprites[i]->playerDist = distance(Lsprites[i]->x,Lsprites[i]->y,player->x, player->y);
	}
	qsort(Lsprites, (size_t)n_sprites, sizeof(Sprite*),cmp_sprites);

}

void drawSprites(SDL_Renderer *gRenderer, SDL_Window* gWindow,Player *player){
	sortSprites(player);
	for(int s = 0; s < n_sprites;s++){
		double spriteX = Lsprites[s]->x - player->x;
		double spriteY = Lsprites[s]->y - player->y;
		double spriteZ = Lsprites[s]->z;
		
		int txt_x;
		float angle = player->angle;
		float txt_ang = fixAngle(-atan2(spriteY, spriteX));
		txt_x = (int)((txt_ang)) % Lsprites[s]->n_texts;

		float CS=cos(angle), SN=sin(angle); //rotate around origin
		float a=spriteY*CS+spriteX*SN; 
		float b=spriteX*CS-spriteY*SN; 
		spriteX=a; spriteY=b;


		spriteX= (spriteX*108.0/spriteY)+(120/2);
		spriteY=(spriteZ*108.0/spriteY)+( 80/2);

		float scale = 32*80/b;
		if(scale<0){ 
			scale=0;
		} 
		if(scale>240){ 
			scale=240;
		}

		float j = 0;
		float diff = scale == 0 ? 0 : 64/(scale*16);
			
		for(int i = (spriteX-scale)*8; i < (((spriteX-scale)*8))+scale*16;i++){
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255,255);
			SDL_Rect text = {j,0,1,64};
			if(i > 0 && i< 960 && b<depth[(int)i/2]){
				renderTexture(gRenderer, Lsprites[s]->texture[txt_x], i,spriteY*8, &text,1,scale*16);
			}
			j+=diff;
		}

	}

}


