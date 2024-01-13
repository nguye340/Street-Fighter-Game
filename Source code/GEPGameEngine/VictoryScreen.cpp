#include "VictoryScreen.h"
#include <iostream>
#include "Game.h"
#include "MainMenuState.h"
#include "GameState.h"

using namespace std;

// Begin GameOverState
void VictoryScreen::Enter()
{
	m_pFont = TTF_OpenFont("Font/HousePaintShadowRegular-ZznZ.ttf", 40);
	Game::Instance()->GetAM()->LoadMusicLocal("Aud/GameOver.mp3", "gameOver");
	Game::Instance()->GetAM()->PlayMusicLocal("gameOver");
	Game::Instance()->GetAM()->LoadMusicLocal("Aud/GameOverVoice.mp3", "GameOverVoice");
	Game::Instance()->GetAM()->PlayMusicLocal("GameOverVoice");
}

void VictoryScreen::Update()
{
	if (Game::Instance()->GetLeftMouse())
	{
		Game::Instance()->GetFSM()->Clean(); // Clear all states, including GameState on bottom.
		//go back to main menu
		Game::Instance()->GetGameManager()->ResetGameManager();

		Game::Instance()->GetFSM()->ChangeState(new MainMenuState());

	}

}


void VictoryScreen::Render()
{
	SDL_Rect bgDestRect;
	bgDestRect.x = bgDestRect.y = 0;
	SDL_GetWindowSize(Game::Instance()->GetWindow(), &bgDestRect.w, &bgDestRect.h);

	//Game::Instance()->GetFSM()->GetStates().front()->Render();
	//SDL_SetRenderDrawBlendMode(Game::Instance()->GetRenderer(), SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 0, 180, 171, 128);

	//draw background 
	//SDL_Rect rect = { 128, 128, 768, 512 };
	//SDL_RenderFillRect(Game::Instance()->GetRenderer(), &rect);

	m_pFont = TTF_OpenFont("Font/AerologicaRegular-K7day.ttf", 100);

	//draw fonts
	string s = "-[. CLEAR .]-";
	if (Game::Instance()->GetGameManager()->CheckGameOver())
	{
		if (!Game::Instance()->GetGameManager()->GetGameStatus())
		{
			s = "! PLAYER LOSE !";
			RenderFont(true, s.c_str(), bgDestRect.w * 0.18, 250);
		}
		else if (Game::Instance()->GetGameManager()->GetGameStatus())
		{
			s = "PLAYER WINS!";
			RenderFont(true, s.c_str(), bgDestRect.w * 0.2, 250);
		}
		else 
		{
			s = "- TIE -";
			RenderFont(true, s.c_str(), bgDestRect.w * 0.2, 250);
		}
		Game::Instance()->GetGameManager()->SetGameOver(false);
	}
	/*
	if (Game::Instance()->GetGameManager()->CheckGameOver())
	{
		m_pFont = TTF_OpenFont("Font/HousePaintShadowRegular-ZznZ.ttf", 80);
		string s1 = "* SCORES *" 
			+ to_string(Game::Instance()->GetGameManager()->GetPlayerScore()) + " : "
			+ to_string(Game::Instance()->GetGameManager()->GetEnemyScore());
		RenderFont(true, s1.c_str(), bgDestRect.w * 0.2, 400);
		Game::Instance()->GetGameManager()->SetGameOver(false);
	}
	*/

	m_pFont = TTF_OpenFont("Font/HousePaintShadowRegular-ZznZ.ttf", 40);
	string s2 = "Click anywhere to restart level";
	RenderFont(true, s2.c_str(), bgDestRect.w*0.15, 700);

	ScreenState::Render();

}


void VictoryScreen::Exit()
{
	Game::Instance()->GetAM()->UnloadMusic();
	Game::Instance()->GetAM()->UnloadSound();
}

