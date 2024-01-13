#include "GameOverState.h"
#include <iostream>
#include "Game.h"
#include "MainMenuState.h"
#include "GameState.h"

using namespace std;

// Begin GameOverState
void GameOverState::Enter()
{
	cout << "Entering GAME END..." << endl;
	m_vButtons.push_back(new Button("Assets/Sprites/replaySprite.png", { 0,0,440, 128 }, { 400,200,220,80 }));
	m_vButtons.push_back(new Button("Assets/Sprites/exit.png", { 0,0,400,100 }, { 400,512,200,50 }));
	m_pFont = TTF_OpenFont("Assets/Fonts/OrbitRegular.ttf", 40);

}

void GameOverState::Update()
{
	// Update buttons. Allows for mouseovers.
	for (int i = 0; i < (int)m_vButtons.size(); i++)
		m_vButtons[i]->Update();

	//else if exit was clicked, we need to go back to main menu
	if (m_vButtons[btn::restart]->Clicked())
	{
		Game::Instance()->GetFSM()->Clean(); // Clear all states, including GameState on bottom.
		//start a new game
		Game::Instance()->GetFSM()->ChangeState(new GameState());
	}
	//else if exit was clicked, we need to go back to main menu
	else if (m_vButtons[btn::exit]->Clicked())
	{
		Game::Instance()->GetFSM()->Clean(); // Clear all states, including GameState on bottom.
		//go back to main menu
		Game::Instance()->GetFSM()->ChangeState(new MainMenuState());
	}


}


void GameOverState::Render()
{

	//Game::Instance()->GetFSM()->GetStates().front()->Render();
	//SDL_SetRenderDrawBlendMode(Game::Instance()->GetRenderer(), SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 0, 180, 171, 128);

	//draw background 
	SDL_Rect rect = { 128, 128, 768, 512 };
	SDL_RenderFillRect(Game::Instance()->GetRenderer(), &rect);

	//draw the buttons
	for (int i = 0; i < (int)m_vButtons.size(); i++)
		m_vButtons[i]->Render();

	//draw fonts
	string s = "-[. CLEAR .]-";
	RenderFont(true, s.c_str(), 380, 320);

	string s1 = "SCORES: " + to_string(highScore);
	RenderFont(true, s1.c_str(), 380, 400);

	ScreenState::Render();
	
}


void GameOverState::Exit()
{
	cout << "Return to main..." << endl;

	for (int i = 0; i < (int)m_vButtons.size(); i++)
	{
		delete m_vButtons[i];
		m_vButtons[i] = nullptr;
	}
	m_vButtons.clear();
	m_vButtons.shrink_to_fit();
}

