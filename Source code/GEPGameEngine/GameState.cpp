#include "GameState.h"
#include "Game.h"
#include "PauseMenuPopupState.h"
#include "GameOverState.h"


void GameState::Enter()
{
	// Load background
	bgSpriteTex = Game::Instance()->LoadTexture("Assets/Sprites/background.png");
	mainSpriteTex = Game::Instance()->LoadTexture("Assets/Sprites/Sprites.png");
	enemySpriteTex = Game::Instance()->LoadTexture("Assets/Sprites/enemySprite.png");
	shieldSpriteTex = Game::Instance()->LoadTexture("Assets/Sprites/shieldSprite.png");
	bonusSpriteTex = Game::Instance()->LoadTexture("Assets/Sprites/bonusSprite.png");

	SDL_Rect bgSrcRect;
	bgSrcRect.x = bgSrcRect.y = 0;
	SDL_QueryTexture(bgSpriteTex, NULL, NULL, &bgSrcRect.w, &bgSrcRect.h);

	SDL_Rect bgDestRect;
	bgDestRect.x = bgDestRect.y = 0;
	SDL_GetWindowSize(Game::Instance()->GetWindow(), &bgDestRect.w, &bgDestRect.h);

	bg = new SpriteEx(bgSpriteTex, bgSrcRect, bgDestRect);

	// Load Font for Scores/Lives
	m_pFont = TTF_OpenFont("Assets/Fonts/spaceNorm.ttf", 35);

	// Create random sized asteroids
	SDL_Rect asteroidSrcRect;
	asteroidSrcRect.x = 124;
	asteroidSrcRect.y = 0;
	asteroidSrcRect.w = asteroidSrcRect.h = 66;//66 pixelsx66 pixels is each cell's dimension

	SDL_Rect asteroidDestRect;

	for (int i = 0; i < 5; i++)
	{
		// generate a random speed
		float r = (1 - rand() % 2 * 2) * (rand() % 6 + 1);

		// randomize asteroid sizes
		asteroidDestRect.w = asteroidDestRect.h = (rand() % 140) + 60;

		// generate asteroid hitbox/radius based on its size
		int asteroidRadius = asteroidDestRect.w * 0.5;
		// 0.495 and not 1/2 to allow a margin error for player's narrow escape

		// randomize asteroid positions
		asteroidDestRect.x = (rand() % 700) + 1; //generate between 1 and 700
		asteroidDestRect.y = (rand() % 600) + 1;//generate between 1 and 600


		Asteroid* ast = new Asteroid(mainSpriteTex, asteroidSrcRect, asteroidDestRect, r, asteroidRadius);
		asteroids.push_back(ast);
	}

	// Create enemies
	for (int i = 0; i < 5; i++)
	{
		Enemy* enemy = new Enemy(enemySpriteTex, (rand() % 700) + 1, (rand() % 700) + 1);
		enemies.push_back(enemy);
	}

	// Create shield based on sprite texture
	shield = new Shield(shieldSpriteTex, (rand() % 700) + 1, (rand() % 600) + 1);

	// Create healthkit based on sprite texture
	for (int i = 0; i < 2; i++)
	{
		HealthKit* hk = new HealthKit(bonusSpriteTex, (rand() % 700) + 1, (rand() % 600) + 1);
		healthKits.push_back(hk);
	}

	// Create multi-bonus based on sprite texture
	for (int i = 0; i < 3; i++)
	{
		Bonus* bonus = new Bonus(bonusSpriteTex, (rand() % 700) + 1, (rand() % 600) + 1);
		bonuses.push_back(bonus);
	}


	player = new Player(mainSpriteTex, bgDestRect.w * 0.5, bgDestRect.h - 100);

	fireSound = Mix_LoadWAV("Assets/Sounds/Fire.wav");
	engineSound = Mix_LoadWAV("Assets/Sounds/Engines.wav");
	Mix_VolumeChunk(fireSound, 50);
	Mix_VolumeChunk(engineSound, 50);
}

void GameState::Update()
{
	if (Game::Instance()->KeyDown(SDL_SCANCODE_ESCAPE) == 1)
	{
		Game::Instance()->GetFSM()->PushState(new PauseMenuPopupState());
		return; //to make sure we don't run the game in background while pause menu shows
	}
	/*
		if (!Game::Instance()->IsRunning())
		{
			Game::Instance()->GetFSM()->PushState(new GameOverState());
			return; //to make sure we don't run the game in background while pause menu shows
		}
		*/
	if (player) player->Update();
	if (shield) shield->Update();

	for (int i = 0; i < asteroids.size(); i++)
		asteroids[i]->Update();

	for (int i = 0; i < enemies.size(); i++)
	{
		enemies[i]->Update();
		enemies[i]->SpawnBullet();
	}

	for (int i = 0; i < healthKits.size(); i++)
	{
		healthKits[i]->Update();
	}

	for (int i = 0; i < bonuses.size(); i++)
	{
		bonuses[i]->Update();
	}


	if (player) this->CheckCollision();

	// Shooting using spacebar
	if (Game::Instance()->KeyDown(SDL_SCANCODE_SPACE) && player)
	{

		if (player->GetIsFired()) {
			Mix_PlayChannel(-1, fireSound, 0);
			//-1 means we ask the mixer to play on the first available channel. the third argument: 0, means we play it just once (no looping)
			player->SpawnBullet();
			player->SetIsFired(false);
		}

	}

	if (Game::Instance()->KeyUp(SDL_SCANCODE_SPACE) && player)
	{
		player->SetIsFired(true);
	}

	//Play engine sound....
	if (Game::Instance()->KeyDown(SDL_SCANCODE_W))
	{
		if (!Mix_Playing(15)) //if channel 15 is not playing
			Mix_PlayChannel(15, engineSound, -1);   //-1 because we want to loop the sound...

	}
	if (Game::Instance()->KeyUp(SDL_SCANCODE_W))
	{
		Mix_FadeOutChannel(15, 250); //250 is in milliseconds.. the time over which the audio will fade out...
	}


}

void GameState::Render()
{
	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 49, 54, 57, 255); //set background window color
	SDL_RenderClear(Game::Instance()->GetRenderer()); //clear the screen

	bg->Render();

	if (player) player->Render();

	if (shield) shield->Render();

	for (int i = 0; i < asteroids.size(); i++)
		asteroids[i]->Render();

	for (int i = 0; i < enemies.size(); i++)
		enemies[i]->Render();

	for (int i = 0; i < healthKits.size(); i++)
		healthKits[i]->Render();

	for (int i = 0; i < bonuses.size(); i++)
		bonuses[i]->Render();

	string s = "SCORES: " + to_string(score);
	if (scoreMulti > 1) {
		s = "SCORES (X" + to_string(scoreMulti) + "):" + to_string(score);
	}

	RenderFont(true, s.c_str(), 32, 20);
	string l = "LIVES: " + to_string(lives);
	if (scoreMulti > 1)	RenderFont(true, l.c_str(), 72, 60);
	else RenderFont(true, l.c_str(), 32, 60);

	ScreenState::Render();
}

void GameState::Exit()
{
	lives = 3;
	score = 0;
	scoreMulti = 0;

	delete shield;
	shield = nullptr;

	delete player;
	player = nullptr;

	for (int i = 0; i < (int)healthKits.size(); i++)
	{
		delete healthKits[i];
		healthKits[i] = nullptr;
		healthKits.erase(healthKits.begin() + i);
	}

	for (int i = 0; i < (int)enemies.size(); i++)
	{
		delete enemies[i];
		enemies[i] = nullptr;
		enemies.erase(enemies.begin() + i);
	}

	for (int i = 0; i < (int)bonuses.size(); i++)
	{
		delete bonuses[i];
		bonuses[i] = nullptr;
		bonuses.erase(bonuses.begin() + i);
	}

	fireSound = nullptr;
	engineSound = nullptr;
	free(fireSound);
	free(engineSound);
}


void GameState::CheckCollision()
{
	if (player) {
		if (shield)
		{
			if (shield->IsShield()) {

				shield->SetSpriteX(player->GetX());
				shield->SetSpriteY(player->GetY());

				if (shield->GetShieldHealth() <= 0)
				{
					delete shield;
					shield = nullptr;

					return;
				}
			}


			// PICK-UP: check for the player and shield collision
			if (CircleCollisionTest(player->GetX(), player->GetY(),
				shield->GetX(), shield->GetY(),
				player->GetRadius(), shield->GetRadius()
			))
			{
				//there was a player-asteroid collision
				cout << "Player equipped with a shield!!\n";
				shield->SetShield(true);

			}

		}

		//check for the player and HEALTH collision
		for (int i = 0; i < (int)healthKits.size(); i++)
		{

			if (CircleCollisionTest(player->GetX(), player->GetY(),
				healthKits[i]->GetX(), healthKits[i]->GetY(),
				player->GetRadius(), healthKits[i]->GetRadius())
				)
			{
				//there was a player-asteroid collision
				cout << "Player got extra health!!\n";
				lives++;


				//player lose immediately with default 3 hp, unless they pick up more hp
				cout << "Remaining life: " << lives << endl;

				//destroy the heart artifacts
				delete healthKits[i];
				healthKits[i] = nullptr;
				healthKits.erase(healthKits.begin() + i);


			}
		}

		//check for the player and BONUS collision
		for (int i = 0; i < (int)bonuses.size(); i++)
		{

			if (CircleCollisionTest(player->GetX(), player->GetY(),
				bonuses[i]->GetX(), bonuses[i]->GetY(),
				player->GetRadius(), bonuses[i]->GetRadius())
				)
			{
				//there was a player-asteroid collision
				cout << "Player got extra health!!\n";
				scoreMulti++;

				//destroy the BONUS artifacts
				delete bonuses[i];
				bonuses[i] = nullptr;
				bonuses.erase(bonuses.begin() + i);


			}
		}

		//check for the player and asteroid collision
		for (int i = 0; i < (int)asteroids.size(); i++)
		{

			if (CircleCollisionTest(player->GetX(), player->GetY(),
				asteroids[i]->GetX(), asteroids[i]->GetY(),
				player->GetRadius(), asteroids[i]->GetRadius())
				)
			{
				//there was a player-asteroid collision
				cout << "Player collided with an asteroid!!\n";
				if (shield) {
					if (shield->IsShield()) {
						shield->decreaseShieldHealth();
					}
					else lives -= asteroids[i]->GetRadius() * 0.02; // size of asteroid from 60-140 (width/height)
				}
				else lives -= asteroids[i]->GetRadius() * 0.02;


				//player lose immediately with default 3 hp, unless they pick up more hp
				cout << "Remaining life: " << lives << endl;


				//destroy the asteroid
				delete asteroids[i];
				asteroids[i] = nullptr;
				asteroids.erase(asteroids.begin() + i);

			}
		}
		// check for player and enemy collision
		for (int i = 0; i < (int)enemies.size(); i++)
		{

			if (CircleCollisionTest(player->GetX(), player->GetY(),
				enemies[i]->GetX(), enemies[i]->GetY(),
				player->GetRadius(), enemies[i]->GetRadius()))

			{
				//there was a player-asteroid collision
				cout << "Player collided with an enemy!!\n";
				if (shield) {
					if (shield->IsShield()) {
						shield->decreaseShieldHealth();
					}
					else lives--;
				}
				else lives --;


				cout << "Remaining life: " << lives << endl;

				//destroy the asteroid
				delete enemies[i];
				enemies[i] = nullptr;
				enemies.erase(enemies.begin() + i);

			}

		}


		bool isBreakOutOfLoop = false;
		// check for bullet and asteroid collision
		for (int b = 0; b < (int)player->GetBullets().size(); b++) {
			for (int i = 0; i < (int)asteroids.size(); i++) {

				Bullet* bullet = player->GetBullets()[b];

				if (CircleCollisionTest(bullet->GetX(), bullet->GetY(),
					asteroids[i]->GetX(), asteroids[i]->GetY(),
					bullet->GetRadius(), asteroids[i]->GetRadius()))
				{
					cout << "Bullet collided with an asteroid!!\n";

					// Add to score, scoreMulti if pick up bonus
					score += 10 * scoreMulti;
					cout << "Current Score: " << score << endl;

					//cleanup/destroy the bullet
					delete bullet;
					player->GetBullets()[b] = nullptr;
					player->GetBullets().erase(player->GetBullets().begin() + b);

					//destroy the asteroid
					delete asteroids[i];
					asteroids[i] = nullptr;
					asteroids.erase(asteroids.begin() + i);

					isBreakOutOfLoop = true;
				}
				if (isBreakOutOfLoop) break;
			}
			if (isBreakOutOfLoop) break;

		}
		if (isBreakOutOfLoop)
		{
			player->GetBullets().shrink_to_fit();
			asteroids.shrink_to_fit();
		}

		bool breakLoop = false;
		// check for BULLET and ENEMY collision
		for (int b = 0; b < (int)player->GetBullets().size(); b++) {
			for (int i = 0; i < (int)enemies.size(); i++) {

				Bullet* bullet = player->GetBullets()[b];

				if (CircleCollisionTest(bullet->GetX(), bullet->GetY(),
					enemies[i]->GetX(), enemies[i]->GetY(),
					bullet->GetRadius(), enemies[i]->GetRadius()))
				{
					cout << "Bullet hit an enemy!!\n";

					// Add to score, scoreMulti if pick up bonus
					score += 10 * scoreMulti;
					cout << "Current Score: " << score << endl;

					//cleanup/destroy the bullet
					delete bullet;
					player->GetBullets()[b] = nullptr;
					player->GetBullets().erase(player->GetBullets().begin() + b);

					//destroy the asteroid
					delete enemies[i];
					enemies[i] = nullptr;
					enemies.erase(enemies.begin() + i);

					breakLoop = true;
				}
				if (breakLoop) break;
			}
			if (breakLoop) break;

		}
		if (breakLoop)
		{
			player->GetBullets().shrink_to_fit();
			enemies.shrink_to_fit();
		}

	}


	//GAME END CHECK
	CheckEndGame();

}

void GameState::CheckEndGame() {

	if ((asteroids.empty() && enemies.empty()) || lives <= 0)
	{
		delete player;
		player = nullptr;

		GameOverState* gameOver = new GameOverState();
		gameOver->SetScore(score);
		Game::Instance()->GetFSM()->PushState(gameOver);
	}

}
