
#include "SDL.h"
#include "Game.h"

int main(int argc, char* argv[])
{
	//in a non-singleton world, we would have had to create a game object like this
	//and we would have used game->Init() or game->HandleEvents() etc
	//but when using a singleton, we don't create an object explicitly..
	//we instead use 'Instance()' function and access the class members via it

	if (!Game::Instance()->Init("SDL Framework", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, 1024, 768, 0))
		return -1; //close the application if initialization fails

//	Game::Instance()->InitializeGame();



	while (Game::Instance()->IsRunning())
	{
		Game::Instance()->HandleEvents();
		Game::Instance()->Update();
		Game::Instance()->Render();
		SDL_Delay(10); //delay of 10 ms after every loop iteration
		

	}
	Game::Instance()->Clean();

	return 0;

}