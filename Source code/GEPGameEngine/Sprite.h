#pragma once
#include "SDL.h"
#include "SDL_image.h"

class Sprite
{
private:
	SDL_Rect spriteSrcRect; //which part of the spritesheet we would like to render
	SDL_Rect spriteDestRect; //the position on the screen we would like to render this sprite at
    SDL_Surface *sprite;  //the handle to the image
	SDL_Texture *texture; //the texture holds the sprite texture after loading it from the sprite
	
	//next week
	//isAnimatable?
	//animSpeed

	int lastColumnNum; //what was the last column we rendered?

	bool isAnimationFinished = false;

	//by default, this will be false. if we need an animated sprite, 
	//we set this to true via constructor
	bool isAnimatable = false;
protected:
//  	int frameNum = 0;  //what frame number we are currently at
//	int frameMax; //how many total frames in a sprite  

	int tRows; //total rows in our sprite animation
	int tColumns; //total columns in our sprite animation

	//together, they define the current cell we are at.. 
	int currentRow;
	int currentColumn;
	int initialRow; //make sure this is 0-based index

	float animSpeed=10; //animation speed, in pixels per change


public:
	Sprite(const char* spritePath, SDL_Rect srcRect, SDL_Rect spritePosition, SDL_Renderer *renderer
	,int totalRows, int totalColumns
		, int initialRowNum, bool animatable //which row we should start playing animation from
	);
	~Sprite();
	void Render();
	void Update(); //to be called from our game.cpp's Update 
};