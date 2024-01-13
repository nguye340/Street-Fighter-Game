#include "Shield.h"
#include "Player.h"
#include "Game.h"

#include <algorithm> //for min/max function
#include "Sprite.h"

Shield::Shield(SDL_Texture* tex, double x, double y)
	: SpriteExAnimated(tex, x, y, 0, 1, 8, 0.001f)
{
	SDL_Point size;
	SDL_QueryTexture(texture, NULL, NULL, &size.x, &size.y);

	spriteSrcRect = { 0,0, 512, 512 };
	spriteDestRect = { (int)(m_X - 50),(int)(m_Y - 160), 64, 64};

	m_dRadius = 30;
	
}

Shield::~Shield()
{

}

void Shield::SetIdle()
{
	spriteSrcRect.y = 0;
	m_iFrame = 0;
	m_iSprite = 0;
}


void Shield::Update()
{
	this->UpdateShield();
}


void Shield::UpdateShield()
{
	if (IsShield()) 
	{
		this->Animate();
		spriteSrcRect = { 0, 512, 678 / 8, 87 }; // general sprite is 670 X 603
		spriteDestRect = { spriteDestRect.x, spriteDestRect.y, 670 / 8 * 2, 87 * 2};
		// minus 100 width and height of player

		m_dRadius = 80;
	
	}

	this->UpdateDestRect();
	spriteSrcRect.x = spriteSrcRect.w * m_iSprite;// update the animation
}

void Shield::SetShield(bool shieldEquipped) {
	isShield = shieldEquipped;
}
bool Shield::IsShield() {
	return isShield;
}

void Shield::SetSpriteX(double targetX)
{
	spriteDestRect.x = (int)targetX - 90;
}

void Shield::SetSpriteY(double targetY) 
{
	spriteDestRect.y = (int)targetY - 100;
}

int Shield::GetShieldHealth() 
{
	return shieldHealth; 
}

void Shield::decreaseShieldHealth()
{
	shieldHealth--;
}