#include "Game.h"
#include "SpriteEx.h"


void SpriteEx::Render()
{
	if (SDL_RenderCopyEx(Game::Instance()->GetRenderer(), texture,
		&spriteSrcRect, &spriteDestRect, angle, nullptr, (isFlipped ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE)) == 0)
	{
		std::cout << "Success...\n";
	}
	else
	{
		std::cout << "Failed to render..\n";
	}
}


SpriteExAnimated::SpriteExAnimated(SDL_Texture* tex, double x, double y,
	double a, int frameMax, int spriteMax, double speed)
{

	texture = tex;
	m_X = x;
	m_Y = y;
	angle = a;
	m_iFrameMax = frameMax;
	m_iSpriteMax = spriteMax;
	m_dSpeed = speed;

}


void SpriteExAnimated::Animate()
{
	m_iFrame++;

	if (m_iFrame >= m_iFrameMax)
	{
		isAnimFinished = true;
		m_iFrame = 0;
		/*
		if (m_iSprite == m_iSpriteMax)
			m_iSprite = 0;
			*/
		if (animStates[currentState].onAnimComplete != nullptr) 
		{
			animStates[currentState].onAnimComplete();
		}

		if (!animStates[currentState].isLoopable)
			currentState = "";

	}


}

void SpriteExAnimated::PlayState(std::string stateName)
{
	if (!animStates[currentState].isLoopable && !isAnimFinished)
	{
		AnimateRange(animStates[currentState]);
		return;
	}
	else if (isAnimFinished) 
	{
		isAnimFinished = false; // reset mechanism
	}
	if (currentState != stateName)  // change of state detected
	{
		m_iFrame = 0; //reset fram number to 0.
		currentState = stateName;
	}
	AnimateRange(animStates[stateName]);
}


void SpriteExAnimated::AnimateRange(AnimStateDefinition asd)
{
	spriteSrcRect.y = (spriteSrcRect.h * asd.rowIndex);

	if (SDL_TICKS_PASSED(SDL_GetTicks(), GetLastTick() + asd.delay))
	{
		UpdateTick();
		m_iFrameMax = asd.frames;
		Animate();
	}

}

void SpriteExAnimated::AddAnimState(std::string stateName, AnimStateDefinition asd)
{
	animStates[stateName] = asd;
	//alternate way to do the above:
	//animStates.insert(std::pair<std::string, AnimStateDefinition>(stateName, asd));
}

void SpriteEx::UpdateDestRect()
{
	spriteDestRect.x = (int)(m_X - spriteDestRect.w*0.5f);
	spriteDestRect.y = (int)(m_Y - spriteDestRect.h*0.5f);
}