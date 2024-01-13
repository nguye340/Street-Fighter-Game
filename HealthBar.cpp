#include "HealthBar.h"
#include "Game.h"
#include "SDL.h"
#include "SDL_image.h"

HealthBar::HealthBar(SDL_Texture* tex, double x, double y, bool isEnemyHealthBar)
	: SpriteExAnimated(tex, x - 50, y - 50, 0, 1, 4, 0.1f)
{
	isFlipped = isEnemyHealthBar;
	spriteSrcRect = { 0, 0, 512, 128 }; // Source location is the background texture of the health bar
	spriteDestRect = { (int)(m_X),(int)(m_Y), 512, 128 };
}

HealthBar::~HealthBar()
{

}

void HealthBar::UpdateCurrentValue(float value)
{

	SDL_Rect bgDestRect;
	bgDestRect.x = bgDestRect.y = 0;
	SDL_GetWindowSize(Game::Instance()->GetWindow(), &bgDestRect.w, &bgDestRect.h);


	currentValue = value;
	if (value == 0)
	{
		setCompleteState(false); // reset the health bar
	}

	if (healthBarFill != NULL && !IsMaxedOut()) // if the health bar has already been populated properly
	{

		// this->spriteDestRect.w or spriteDestRect.w is the width of the health bar background
		// the max width of the empty health bar background is 512 - correspondent to the establish max value
		// Calculate the percentage of current value vs max value, then times 512 or spriteDestRect.w to get the correspondent width on screen of the fill texture to current value
		float temp = (512 - 119) * ((currentValue)/(maxValue + 5)); // +5 is buffer for the edge of the background

		healthBarFill->spriteSrcRect = { 119, 128, (int)temp, 128 }; // + 10 is the cushion to cover the edges of the health bar.
		healthBarFill->spriteDestRect.w = (int)temp;
		
		if (healthBarFill->isFlipped)
		{
			healthBarFill->spriteDestRect.x = (bgDestRect.w - 68) - (healthBarFill->spriteDestRect.w);
		}
		else
		{
			healthBarFill->spriteDestRect.x = 75;
		}

		if (healthBarFill->spriteDestRect.w >= this->spriteDestRect.w) // check if fill texture overflow the background or not
		{
			//OnHealthBarComplete();
		}
	}
}

void HealthBar::Render()
{
	this->SpriteExAnimated::Render(); //invoke the base class's Render()
	if (healthBarFill)
	{
		healthBarFill->SpriteExAnimated::Render();
	}

}

void HealthBar::SetMaxValue(float maxVal)
{
	maxValue = maxVal;
}

void HealthBar::OnHealthBarComplete()
{
	UpdateCurrentValue(0);
	currentValue = 0;
	isMaxed = true;
}

bool HealthBar::IsMaxedOut()
{
	if (isMaxed == NULL)
	{
		isMaxed = false;
	}
	return isMaxed;
}
