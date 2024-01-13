#include "GameLevel3.h"
#include "Game.h"

void GameLevel3::Enter()
{
	bgSpriteTex = Game::Instance()->LoadTexture("Img/Levels/SF_Level_3.png");
	Game::Instance()->GetAM()->LoadMusicLocal("Aud/game3.mp3", "game3");
	Game::Instance()->GetAM()->PlayMusicLocal("game3");
	//call base class' function
	//call base class' function
	GameState::Enter();
}
void GameLevel3::Exit()
{
	Game::Instance()->GetAM()->UnloadMusic();
	Game::Instance()->GetAM()->UnloadSound();
}