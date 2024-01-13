#pragma once

#include "ScreenState.h"
#include "Game.h"

void ScreenState::Render()
{
	SDL_RenderPresent(Game::Instance()->GetRenderer());

}

void ScreenState::RenderFont(bool isRefreshText, const char *text, int x, int y)
{

	if (isRefreshText)
	{
		SDL_Color textColor = { 255,255,255,0 };//white color
		SDL_Surface * fontSurface = TTF_RenderText_Solid(m_pFont, text, textColor);

		SDL_DestroyTexture(m_pFontTexture); //need to de-allocate previous font texture
		m_pFontTexture = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), fontSurface);
		m_rFontRect = { x,y,fontSurface->w,fontSurface->h };

	}

	SDL_RenderCopy(Game::Instance()->GetRenderer(), m_pFontTexture, 0, &m_rFontRect);

}