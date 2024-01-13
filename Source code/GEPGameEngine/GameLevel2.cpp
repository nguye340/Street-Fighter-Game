#include "GameLevel2.h"
#include "Game.h"

void GameLevel2::Enter()
{
	bgSpriteTex = Game::Instance()->LoadTexture("Img/Levels/SF_Level_2b.png");
	Game::Instance()->GetAM()->LoadMusicLocal("Aud/game2.mp3", "game2");
	Game::Instance()->GetAM()->PlayMusicLocal("game2");
	//call base class' function
	//call base class' function
	GameState::Enter();
}
void GameLevel2::Exit()
{
	Game::Instance()->GetAM()->UnloadMusic();
	Game::Instance()->GetAM()->UnloadSound();
}