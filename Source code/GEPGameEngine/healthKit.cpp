#include "HealthKit.h"
#include "Player.h"
#include "Game.h"

#include <algorithm> //for min/max function
#include "Sprite.h"

HealthKit::HealthKit(SDL_Texture* tex, double x, double y)
	: SpriteExAnimated(tex, x, y, 0, 1, 8, 0.1f)
{
	SDL_Point size;
	SDL_QueryTexture(texture, NULL, NULL, &size.x, &size.y);

	spriteSrcRect = { 0, 128, 180,  256};
	spriteDestRect = { (rand() % 700) + 1, (rand() % 600) + 1, 64, 64};

	m_dRadius = 32;

}

HealthKit::~HealthKit()
{

}

void HealthKit::Update()
{
	this->UpdateHealthKit();
}


void HealthKit::UpdateHealthKit()
{
	//this->Animate();
	this->UpdateDestRect();
	spriteSrcRect.x = spriteSrcRect.w * m_iSprite;// update the animation
}

