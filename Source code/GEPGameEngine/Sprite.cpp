#include "Sprite.h"
#include "Game.h"

#include <iostream>
Sprite::Sprite(const char* spritePath, SDL_Rect srcRect, SDL_Rect spritePosition, SDL_Renderer *renderer
	, int totalRows, int totalColumns, int initialRowNum, bool animatable
)
{
	tRows = totalRows;
	tColumns = totalColumns;
	initialRow = initialRowNum;
	isAnimatable = animatable; 

	sprite = IMG_Load(spritePath);
		spriteDestRect = spritePosition;
	spriteSrcRect = srcRect;
	texture = SDL_CreateTextureFromSurface(renderer, sprite);

	//set the the y of the source rect to this::
	spriteSrcRect.y = spriteSrcRect.h*initialRow;  
}

Sprite::~Sprite()
{
//	SDL_FreeSurface(sprite); 
//	SDL_DestroyTexture(texture); //destroy the texture
}

//Renders the sprite.
//Must be called in the main game render loop
void Sprite::Render()
{

	if (SDL_RenderCopy(Game::Instance()->GetRenderer() , texture,
		&spriteSrcRect, &spriteDestRect) == 0)
		std::cout << "Success...\n";
	else
		std::cout << "Failed..\n";
}

//called in a loop via game.cpp's Update
void Sprite::Update()
{
	if (!isAnimatable) return;

	if (isAnimationFinished) return;

		///convert getticks() value from milliseconds to seconds 
	//also multiply with the animSpeed to adjust the speed of the sprite anim
	Uint32 fps = (SDL_GetTicks() / 1000.0f) * animSpeed;

	currentColumn = fps  % tColumns;

	//if this happens, it means we have cycled through an entire row
	if (currentColumn < lastColumnNum)
	{
		currentRow++; //it's time to move to the next row

 //that means we were at the last row... so it's time to set back to the initial row num and start animation from beginning
		if (currentRow >= tRows)
		{
			currentRow = initialRow;  //the initialRow value is passed from the main class where we initialize the sprite actor.

		//if the sprite is marked as 'not loopable' then do this...
			isAnimationFinished = true;
		}

	}


	spriteSrcRect.x = spriteSrcRect.w * currentColumn;
	spriteSrcRect.y = spriteSrcRect.h * currentRow;

	lastColumnNum = currentColumn;
}

