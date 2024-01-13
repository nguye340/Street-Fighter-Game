#include "Button.h"
#include "Game.h"
#include "SDL.h"
#include "SDL_image.h"

Button::Button(const char* s, SDL_Rect src, SDL_Rect dst)
{
	// Set the button image. You should have some fail checking just in case. 
	m_pSurf = IMG_Load(s);
	m_pText = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), m_pSurf);
	SDL_FreeSurface(m_pSurf);
	// Do the rest.
	m_bReleased = true;
	m_rSrc = src;
	m_rDst = dst;
}

Button::~Button()
{
	SDL_DestroyTexture(m_pText);
}

void Button::Update()
{
	int mx = Game::Instance()->GetMouseX();
	int my = Game::Instance()->GetMouseY();
	if (mx < (m_rDst.x + m_rDst.w) && mx > m_rDst.x && // If cursor is within X bounds
		my < (m_rDst.y + m_rDst.h) && my > m_rDst.y)   // And cursor is within Y bounds
	{
		if (Game::Instance()->GetLeftMouse() && m_bReleased)
		{
			m_iFrame = MOUSE_DOWN;
			m_bReleased = false;
		}
		else if (!Game::Instance()->GetLeftMouse())
		{
			m_bReleased = true;
			m_iFrame = MOUSE_OVER;
		}
	}
	else
		m_iFrame = MOUSE_UP;
}

void Button::Render()
{
	m_rSrc.x = m_rSrc.w * m_iFrame;
	SDL_RenderCopy(Game::Instance()->GetRenderer(), m_pText, &m_rSrc, &m_rDst);
}