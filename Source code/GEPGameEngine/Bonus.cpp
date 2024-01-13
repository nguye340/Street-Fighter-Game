#include "Bonus.h"
#include "Player.h"
#include "Game.h"

#include <algorithm> //for min/max function
#include "Sprite.h"

Bonus::Bonus(SDL_Texture* tex, double x, double y)
	: SpriteExAnimated(tex, x, y, 0, 1, 5, 0.1f)
{
	SDL_Point size;
	SDL_QueryTexture(texture, NULL, NULL, &size.x, &size.y);

	spriteSrcRect = { 0, 0, 128,  128 };
	spriteDestRect = { (rand() % 700) + 1, (rand() % 600) + 1, 64, 64 };

	m_dRadius = 32;

}

Bonus::~Bonus()
{

}

void Bonus::SetIdle()
{
	spriteSrcRect.y = 0;
	m_iFrame = 0;
	m_iSprite = 0;
}


void Bonus::Update()
{
	this->UpdateBonus();
}


void Bonus::UpdateBonus()
{
	this->Animate();
	this->UpdateDestRect();
	spriteSrcRect.x = spriteSrcRect.w * m_iSprite;// update the animation
}

void Bonus::SetBonus(bool shieldEquipped) {
	isBonus = shieldEquipped;
}
bool Bonus::IsBonus() {
	return isBonus;
}

void Bonus::SetSpriteX(double targetX)
{
	spriteDestRect.x = (int)targetX - 90;
}

void Bonus::SetSpriteY(double targetY)
{
	spriteDestRect.y = (int)targetY - 100;
}

int Bonus::GetBonusHealth()
{
	return shieldHealth;
}

void Bonus::decreaseBonusHealth()
{
	shieldHealth--;
}