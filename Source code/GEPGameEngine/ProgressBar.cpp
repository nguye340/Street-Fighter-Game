#include "ProgressBar.h"
#include "Game.h"
#include "SDL.h"
#include "SDL_image.h"

ProgressBar::ProgressBar(SDL_Texture* tex, double x, double y)
	: SpriteExAnimated(tex, x - 50, y - 50, 0, 1, 4, 0.1f)
{

	spriteSrcRect = { 0, 0, 512, 64 }; // Source location is the background texture of the progress bar
	spriteDestRect = { (int)(m_X),(int)(m_Y), 512, 64 };
}

ProgressBar::~ProgressBar()
{

}

void ProgressBar::UpdateCurrentValue(float value)
{

	currentValue = value;
	if (value == 0)
	{
		setCompleteState(false); // reset the progress bar
	}

	if (progressBarFill != NULL && !IsMaxedOut()) // if the progress bar has already been populated properly
	{

		// this->spriteDestRect.w or spriteDestRect.w is the width of the progress bar background
		// the max width of the empty progress bar background is 512 - correspondent to the establish max value
		// Calculate the percentage of current value vs max value, then times 512 or spriteDestRect.w to get the correspondent width on screen of the fill texture to current value
		float temp = this->spriteDestRect.w * (currentValue / maxValue);

		progressBarFill->spriteSrcRect = { 0, 64, (int)temp + 10, 64 }; // + 10 is the cushion to cover the edges of the progress bar.
		progressBarFill->spriteDestRect.w = (int)temp + 10;

		if (progressBarFill->spriteDestRect.w >= this->spriteDestRect.w) // check if fill texture overflow the background or not
		{
			OnProgressBarComplete();
		}
	}
}

void ProgressBar::Render()
{
	this->SpriteExAnimated::Render(); //invoke the base class's Render()
	if (progressBarFill)
	{
		progressBarFill->SpriteExAnimated::Render();
	}

}

void ProgressBar::SetMaxValue(float maxVal)
{
	maxValue = maxVal;
}

void ProgressBar::OnProgressBarComplete()
{
	UpdateCurrentValue(0);
	currentValue = 0;
	isMaxed = true;
}

bool ProgressBar::IsMaxedOut()
{
	if (isMaxed == NULL)
	{
		isMaxed = false;
	}
	return isMaxed;
}
