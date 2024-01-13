#include "MainMenuState.h"
#include "Game.h"
#include "GameState.h"

void MainMenuState::Enter()
{
	cout << "Entering MainMenu...\n";
	Game::Instance()->SetLeftMouse(false);

	m_vButtons.push_back(new Button("Assets/Sprites/play.png", { 0,0,400,100 }, { 312,200,400,100 }));
	m_vButtons.push_back(new Button("Assets/Sprites/exit.png", { 0,0,400,100 }, { 312,400,400,100 }));
}

void MainMenuState::Update()
{

	// Update buttons. Allows for mouseovers.
	for (int i = 0; i < (int)m_vButtons.size(); i++)
		m_vButtons[i]->Update();

	if (m_vButtons[btn::play]->Clicked())
	{
		//we need to switch to gamestate ... 
		Game::Instance()->GetFSM()->ChangeState(new GameState());
	}

	else if (m_vButtons[btn::exit]->Clicked())
	{
		SDL_Delay(500);
		Game::Instance()->QuitGame();
	}

}


void MainMenuState::Render()
{

	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 106, 27, 224, 255);
	SDL_RenderClear(Game::Instance()->GetRenderer());
	for (int i = 0; i < (int)m_vButtons.size(); i++)
		m_vButtons[i]->Render();

	ScreenState::Render();
}

void MainMenuState::Exit()
{
	cout << "Exiting MainMenu...\n";

	for (int i = 0; i < (int)m_vButtons.size(); i++)
	{
		delete m_vButtons[i];
		m_vButtons[i] = nullptr;
	}
	m_vButtons.clear();
	m_vButtons.shrink_to_fit();

}