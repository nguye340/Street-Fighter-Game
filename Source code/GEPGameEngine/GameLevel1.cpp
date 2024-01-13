#include "GameLevel1.h"
#include "Game.h"

void GameLevel1::Enter() 
{
	bgSpriteTex = Game::Instance()->LoadTexture("Img/Levels/SF_Level_1.jpg");
	Game::Instance()->GetAM()->LoadMusicLocal("Aud/game.mp3","game");
	Game::Instance()->GetAM()->PlayMusicLocal("game");
	//call base class' function
	GameState::Enter();
}
void GameLevel1::Exit()
{
	Game::Instance()->GetAM()->UnloadMusic();
	Game::Instance()->GetAM()->UnloadSound();
}