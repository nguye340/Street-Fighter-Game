#include <iostream>
#include "Game.h"
#include "MainMenuState.h"
#include "AudioManager.h"

using namespace std;

//Singleton pattern for Game class 
//is defined as follows
Game* Game::Instance()
{
	static Game* instance = new Game();
	return instance;
}

bool Game::Init(const char* title, const int xPos, const int yPos, const int width, const int height,
	const int flags)
{
	//0 means success
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		cout << "SDL Initialization successful\n";

		sdlWindow = SDL_CreateWindow(title, xPos, yPos, width, height, flags);
		if (sdlWindow == nullptr)
		{
			cout << "Window initialization failed.\n";
			return false;
		}
		cout << "Window successfully created.\n";
		sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, 0);
		if (sdlRenderer == nullptr)
		{
			cout << "renderer initialization failed.\n";
			return false;
		}
	}
	else
	{
		cout << "SDL Initialization failed\n";
		return false;
	}


	if (TTF_Init() == 0) //font init is succesful
	{
		cout << "Font init success!\n";
	}
	else
	{
		cout << "Font init fail!\n";
		return false;
	}

	m_pAM = new AudioManager();
	if (!m_pAM->Init())
	{
		// if audio not initialize, return false
		return false;
	}

	// set bgm small to hear sfx better
	

	m_iKeystates = SDL_GetKeyboardState(nullptr);
	isAppRunning = true;

	m_pFSM = new StateMachine();	
	m_pGameManager = new GameManager();

	m_pFSM->ChangeState(new MainMenuState());


	return true;
}


SDL_Texture* Game::LoadTexture(const char* path)
{
	SDL_Surface *surface = IMG_Load(path);
	return SDL_CreateTextureFromSurface(sdlRenderer, surface);

}


 
bool Game::IsRunning()
{
	return isAppRunning;
}

//check whether a specific key, passed as argument 'c'
//is pressed down
bool Game::KeyDown(SDL_Scancode c)
{
	if (m_iKeystates == nullptr)
		return false;

	if (m_iKeystates[c] == 1)
		return true;
	else return false;
}

bool Game::KeyUp(SDL_Scancode c)
{
	if (m_iKeystates == nullptr)
		return false;

	if (m_iKeystates[c] == 0)
		return true;
	else return false;
}



void Game::Update()
{

	GetFSM()->Update();
	GetGameManager()->Update();
}


void Game::HandleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			isAppRunning = false;
			break;
	
		case SDL_MOUSEMOTION:
			SDL_GetMouseState(&m_iMouseX, &m_iMouseY);
			break;

		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT)
				m_bLeftMouse = true;
			break;

		case SDL_MOUSEBUTTONUP:
		{
			if (event.button.button == SDL_BUTTON_LEFT)
				m_bLeftMouse = false;

			//if (event.button.button == SDL_BUTTON_LEFT && player)
			//{
			//	//do something...  such as fire a bullet/missile
			//	//and play sound
			//	Mix_PlayChannel(-1, fireSound, 0); //-1 means we ask the mixer to play on the first available channel. the third argument: 0, means we play it just once (no looping)
			//	player->SpawnBullet();
			//}
		}
		break;

		}
	}
}


void Game::Render()
{
	GetFSM()->Render();
}

void Game::Clean()
{
	cout << "Cleaning up and shutting down engine...\n";

	delete m_pFSM;
	delete m_pAM;

	Instance()->GetAM()->UnloadMusic();
	Instance()->GetAM()->UnloadSound();
	
	SDL_DestroyWindow(sdlWindow);
	SDL_DestroyRenderer(sdlRenderer);

	//TTF_CloseFont(m_pFont);
	TTF_Quit();

	IMG_Quit();

	SDL_Quit();

}


