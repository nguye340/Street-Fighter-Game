#include "GameState.h"
#include "Game.h"
#include "PauseMenuPopupState.h"
void GameState::Enter()
{
	//bgSpriteTex = Game::Instance()->LoadTexture("Img/Levels/SF_Level_1.jpg");
	mainSpriteTex = Game::Instance()->LoadTexture("Img/Players/PlayerKenSprite2.png");
	enemySpriteTex = Game::Instance()->LoadTexture("Img/Players/EnemyKenSprite.png");


	m_pFont = TTF_OpenFont("Font/spaceNorm.ttf", 35);

	SDL_Rect bgSrcRect;
	bgSrcRect.x = bgSrcRect.y = 0;
	SDL_QueryTexture(bgSpriteTex, NULL, NULL, &bgSrcRect.w, &bgSrcRect.h);

	SDL_Rect bgDestRect;
	bgDestRect.x = bgDestRect.y = 0;
	SDL_GetWindowSize(Game::Instance()->GetWindow(), &bgDestRect.w, &bgDestRect.h);

	bg = new SpriteEx(bgSpriteTex, bgSrcRect, bgDestRect);

	player = new Player(mainSpriteTex, bgDestRect.w * 0.35, bgDestRect.h - 100);
	
	enemy = new Enemy(enemySpriteTex, bgDestRect.w * 0.75, bgDestRect.h - 100);

	// scaling up enemy's speed by levels to raise difficulty
	enemy->ScaleSpeedByLevel(Game::Instance()->GetGameManager()->GetLevel());

	// Load texture of progress bar
	healthBarTex = Game::Instance()->LoadTexture("Img/healthBarSprite.png");

	// Create new instance of player healthbar where its background is loaded.
	playerHealthBar = new HealthBar(healthBarTex, 5, 60, false);
	playerHealthBar->healthBarFill = new HealthBar(healthBarTex, 5, 60, false);

	// Create new instance of enemy where its background is loaded.
	enemyHealthBar = new HealthBar(healthBarTex, bgDestRect.w * 0.6, 60, true);
	enemyHealthBar->healthBarFill = new HealthBar(healthBarTex, bgDestRect.w * 0.6, 60, true);

}


void GameState::Update()
{
	if (Game::Instance()->KeyDown(SDL_SCANCODE_ESCAPE) == 1)
	{
		Game::Instance()->GetFSM()->PushState(new PauseMenuPopupState());
		return; //to make sure we don't run the game in background while pause menu shows
	}

	if (player) player->Update();
	if (enemy) enemy->Update();
 

	if (player) this->CheckCollision();
 
	// Enemy Movement AI
	enemy->SetTargetPositionX(player->GetX() + player->GetRadius());

	// HealthBar
	if (playerHealthBar)
	{
		playerHealthBar->SetMaxValue(100);
		playerHealthBar->UpdateCurrentValue(Game::Instance()->GetGameManager()->GetPlayerHealth());
	}
	if (enemyHealthBar)
	{
		enemyHealthBar->SetMaxValue(100);
		enemyHealthBar->UpdateCurrentValue(Game::Instance()->GetGameManager()->GetEnemyHealth());
	}

}


void GameState::Render()
{
	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 0, 0, 0, 255); //set background window color
	SDL_RenderClear(Game::Instance()->GetRenderer()); //clear the screen

	bg->Render();

	if (player) player->Render();

	if (enemy) enemy->Render();

	if (playerHealthBar) playerHealthBar->Render();

	if (enemyHealthBar) enemyHealthBar->Render();

	SDL_Rect bgDestRect;
	bgDestRect.x = bgDestRect.y = 0;
	SDL_GetWindowSize(Game::Instance()->GetWindow(), &bgDestRect.w, &bgDestRect.h);

	string s = to_string(Game::Instance()->GetGameManager()->GetPlayerScore()) 
		+ " : " + to_string(Game::Instance()->GetGameManager()->GetEnemyScore());
	
	RenderFont(true, s.c_str(), bgDestRect.w * 0.47, 60);

	if (Game::Instance()->GetGameManager()->GetLevel() <= 3)
	{
		string l = "Lvl. " + to_string(Game::Instance()->GetGameManager()->GetLevel());
		RenderFont(true, l.c_str(), bgDestRect.w * 0.45, 20);
	}
	if (Game::Instance()->GetGameManager()->GetLevel() > 3)
	{
		string l = "Lvl. 3";
		RenderFont(true, l.c_str(), bgDestRect.w * 0.45, 20);
	}
	
	m_pFont = TTF_OpenFont("Font/CurvedSquare-maRv.ttf", 35);
	
	string r = "";
	switch (Game::Instance()->GetGameManager()->GetRound())
	{
	case 1:
		r = "1 . .";
		break;
	case 2:
		r = "1 2 .";
		break;
	case 3:
		r = "1 2 3";
		break;
	}
	
	RenderFont(true, r.c_str(), bgDestRect.w * 0.45, 110);
	m_pFont = TTF_OpenFont("Font/spaceNorm.ttf", 20);

	string totalCtr = "Total Victory: ";
	totalCtr = "Total Victory: " + to_string(Game::Instance()->GetGameManager()->GetPlayerSubScore())
				+ " (rounds)";
	cout << Game::Instance()->GetGameManager()->GetPlayerSubScore();
	RenderFont(true, totalCtr.c_str(), bgDestRect.w * 0.36, bgDestRect.h * 0.22);

	m_pFont = TTF_OpenFont("Font/spaceNorm.ttf", 35);
	ScreenState::Render();


}


void GameState::Exit()
{
	//to-do:: de-allocate memory, texture, surface etc, sound 
	delete enemy;
	enemy = nullptr;

	delete player;
	player = nullptr;

	Game::Instance()->GetAM()->UnloadMusic();
	Game::Instance()->GetAM()->UnloadSound();
}

void GameState::CheckCollision()
{
	SDL_Rect bgDestRect;
	bgDestRect.x = bgDestRect.y = 0;
	SDL_GetWindowSize(Game::Instance()->GetWindow(), &bgDestRect.w, &bgDestRect.h);

	// Check player collision with enemy
	bool enemyPlayerCollided = CircleCollisionTest(player->GetX(), player->GetY(), enemy->GetX(), enemy->GetY(), player->GetRadius(), enemy->GetRadius());
	if (enemyPlayerCollided && player->GetX() - enemy->GetX() < 0)
	{
		enemy->CollisionCheck(true);
		if (player->GetDamagePoint() > 0 && !enemy->IsDefending())
		{
			Game::Instance()->GetGameManager()->EnemyTakeDamage(player->GetDamagePoint());
		}
		
		Game::Instance()->GetGameManager()->PlayerTakeDamage(enemy->GetDamagePoint());
	} 
	else 
	{
		enemy->CollisionCheck(false);
		enemy->SetDamagePoint(0);
		if (!enemy->LineOfSightCheck())
		{
			enemy->SetDamagePoint(0);
		}
	}


	// Check player collision with screen
	if (player->GetX() <= bgDestRect.w * 0.05 || player->GetX() >= bgDestRect.w * 0.96)
	{
		player->Enable(false);
	}
	else player->Enable(true);


	// Check enemy collision with screen
	if (enemy->GetX() <= bgDestRect.w * 0.05 || enemy->GetX() >= bgDestRect.w * 0.96)
	{
		enemy->Enable(false);
	}
	else enemy->Enable(true);	


	bool isBreakOutOfLoop = false;

	bool breakLoop = false;
	// check for PROJECTILE and ENEMY collision
	for (int p = 0; p < (int)player->GetProjectiles().size(); p++) {
		
		Projectile* projectile = player->GetProjectiles()[p];
		player->GetProjectiles()[p]->Update();

		if (CircleCollisionTest(projectile->GetX(), projectile->GetY(),
			enemy->GetX(), enemy->GetY(),
			projectile->GetRadius(), enemy->GetRadius()))
		{
			cout << "Projectile hit an enemy!!\n";

			//cleanup/destroy the projectile
			player->DestroyProjectile(p);

			player->SetDamagePoint(30); //ensure that other consecutive attack doesn't change the damage point of projectiles already fired
			Game::Instance()->GetGameManager()->EnemyTakeDamage(player->GetDamagePoint());
			
			breakLoop = true;
			
			break;
		}
		if (breakLoop) break;
	}
	

	if (breakLoop)
	{
		player->GetProjectiles().clear();
		player->GetProjectiles().shrink_to_fit();
		//enemies.shrink_to_fit();
	}


	// Check for PROJECTILE and PLAYER collision
	isBreakOutOfLoop = false;

	breakLoop = false;
	// check for PROJECTILE and ENEMY collision
	for (int p = 0; p < (int)enemy->GetProjectiles().size(); p++) {

		Projectile* projectile = enemy->GetProjectiles()[p];
		enemy->GetProjectiles()[p]->Update();

		if (CircleCollisionTest(projectile->GetX(), projectile->GetY(),
			player->GetX(), player->GetY(),
			projectile->GetRadius(), player->GetRadius()))
		{
			cout << "Projectile hit a player!!\n";

			//cleanup/destroy the projectile
			enemy->DestroyProjectile(p);

			enemy->SetDamagePoint(30); //ensure that other consecutive attack doesn't change the damage point of projectiles already fired
			Game::Instance()->GetGameManager()->PlayerTakeDamage(enemy->GetDamagePoint());

			breakLoop = true;

			break;
		}
		if (breakLoop) break;
	}


	if (breakLoop)
	{
		enemy->GetProjectiles().clear();
		enemy->GetProjectiles().shrink_to_fit();
		//enemies.shrink_to_fit();
	}

}
