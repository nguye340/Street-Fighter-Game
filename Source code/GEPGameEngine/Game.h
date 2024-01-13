#pragma once
#include "SpriteEx.h"
#include "SDL.h"
#include "SDL_image.h"

#include "SDL_mixer.h"  //required for audio

#include <vector>
#include "Asteroid.h"
#include "Player.h"
#include "SDL_ttf.h"
#include <string>

#include "StateMachine.h"

using namespace std;

class Game
{
private:
	bool isAppRunning;
	SDL_Window* sdlWindow;
	SDL_Renderer* sdlRenderer;
	const Uint8* m_iKeystates; //manages keystates

	StateMachine* m_pFSM;

	bool m_bLeftMouse = false; // Keeps track of left mouse button state
	bool m_bRightMouse = false; // Keeps track of right mouse button state
	int m_iMouseX, m_iMouseY; // Variables to hold mouse positions


public:
	static Game* Instance(); //singleton instance reference
	SDL_Window* GetWindow() { return sdlWindow; }
	SDL_Renderer* GetRenderer() { return sdlRenderer; }
	SDL_Texture* LoadTexture(const char* path);

	//void InitializeActor(int x, int y,int width, int height);
	//void InitializeSpriteActor(const char* spriteLoadPath, int initialRowNum, int totalColumns, int totalRows,
	//	int sheetWidth, int sheetHeight,
	//	int x, int y, int w, int h, bool animatable);

//	void RenderActor(float r,float g, float b,float a);


	//window initialization happens here
	bool Init(const char* title, const int xPos,
		const int yPos, const int width, const int height,
		const int flags);

	void QuitGame() { isAppRunning = false; }
	void InitializeGame();

	//Is the Application running
	bool IsRunning();
	//Check for 'KeyDown' press
	bool KeyDown(SDL_Scancode c);
	bool KeyUp(SDL_Scancode c);

	//The update loop
	void Update();
	//Handles input events
	void HandleEvents();
	//handles rendering
	void Render();
	//clean-up code has been handled by StateMacine clean(), gamestate.exit
	void Clean();

	int GetMouseX() { return m_iMouseX; }
	int GetMouseY() { return m_iMouseY; }
	int GetLeftMouse() { return m_bLeftMouse; }
	void SetLeftMouse(bool b) { m_bLeftMouse = b; }
	int GetRightMouse() { return m_bRightMouse; }
	void SetRightMouse(bool b) { m_bRightMouse = b; }

	StateMachine* GetFSM() { return m_pFSM; }
};