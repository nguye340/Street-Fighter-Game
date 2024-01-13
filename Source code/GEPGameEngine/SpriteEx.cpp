#include "Game.h"


void SpriteEx::Render()
{
	if (SDL_RenderCopyEx(Game::Instance()->GetRenderer(), texture,
		&spriteSrcRect, &spriteDestRect,angle,nullptr,SDL_FLIP_NONE) == 0)
		std::cout << "Success...\n";
	else
		std::cout << "Failed to render..\n";

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

	if (m_iFrame == m_iFrameMax)
	{
		m_iFrame = 0;
		m_iSprite++;
		if (m_iSprite == m_iSpriteMax)
			m_iSprite = 0;

	}


}

void SpriteEx::UpdateDestRect()
{
	spriteDestRect.x = (int)(m_X - spriteDestRect.w*0.5f);
	spriteDestRect.y = (int)(m_Y - spriteDestRect.h*0.5f);
}