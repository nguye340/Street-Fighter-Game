#include "Enemy.h"
#include "Game.h"
#include <time.h>

#include <algorithm> //for min/max functions


Enemy::Enemy(SDL_Texture* tex, double x, double y)
	: SpriteExAnimated(tex, x - 50, y - 50, 0, 1, 4, 0.1f)
{

	spriteSrcRect = { 0, 0, 70, 80 };
	spriteDestRect = { (int)(m_X - 50),(int)(m_Y - 50), 70, 80 };
	isFlipped = true;

	m_velX = 0.5; // default 4 time slower than player
	m_velY = 3.5f;
	m_dRadius = 25;

	AddAnimState("Hadouken", AnimStateDefinition(0, 4, 120));
	AddAnimState("Idle", AnimStateDefinition(1, 4, 120, true));
	AddAnimState("Punch", AnimStateDefinition(2, 3, 120));
	AddAnimState("Move", AnimStateDefinition(3, 5, 120, true));

	AddAnimState("Kick", AnimStateDefinition(6, 5, 120));
	AddAnimState("Roundhouse", AnimStateDefinition(7, 5, 120));
	AddAnimState("Jump", AnimStateDefinition(8, 7, 120));
	AddAnimState("Crouch", AnimStateDefinition(9, 1, 120));

	animStates["Jump"].RegisterOnAnimCompleteCallback(
		std::bind(&Enemy::OnJumpAnimationComplete, this));


	animStates["Hadouken"].RegisterOnAnimCompleteCallback(
		std::bind(&Enemy::SpawnProjectiles, this));
}

Enemy::~Enemy()
{
	for (int i = 0; i < (int)m_vProjectiles.size(); i++)
	{
		delete m_vProjectiles[i];
		m_vProjectiles[i] = nullptr;
	}
	m_vProjectiles.clear();
	m_vProjectiles.shrink_to_fit();
}


void Enemy::Render()
{
	//rendering projectiles here
	for (int i = 0; i < (int)m_vProjectiles.size(); i++)
		m_vProjectiles[i]->Render();
	
	this->SpriteExAnimated::Render(); //invoke the base class's Render()

}

void Enemy::Update()
{
	this->UpdateEnemy();
	this->UpdateProjectiles();
}


void Enemy::Enable(bool isEnabledCheck)
{
	// disable when enemy about to go off-screen
	isEnabled = isEnabledCheck;
}


void Enemy::MoveEnemy()
{   
	bool isFwd = true; // moving forward by default at start of game

	SDL_Rect bgDestRect;
	bgDestRect.x = bgDestRect.y = 0;
	SDL_GetWindowSize(Game::Instance()->GetWindow(), &bgDestRect.w, &bgDestRect.h);
	
	//since the sprite is flipped, player's backward direction is enemy's forward direction 
	if (LineOfSightCheck())  isFwd = false; 
	else isFwd = true;

	// make sure enemy can't go off-screen
	if (isEnabled)
	{
		if (isFwd)
		{
			m_X += m_velX;
			cout << "moving backward";
		}
		else
		{
			m_X -= m_velX;
			cout << "moving forward" ;
		}
	}
	else if (m_X < bgDestRect.w * 0.5) // if character disabled and current position is left of the screen
	{
		Enable(true);
		m_X++; 
	}
	else {
		Enable(true);
		m_X--; //  ensure m_X less then screen size * 0.95 - gameState->CheckCollision()
	}
	
	if (!playerDetected && isMoving) 
	{
			animStates["Move"].isLoopable = false;
			this->PlayState("Move");
	}
	else 
		// stop moving animation when enemy collide with the player
		animStates["Move"].isLoopable = true; 	
}

void Enemy::SetTargetPositionX(double playerPos)
{
	targetPositionX = playerPos;
}

void Enemy::Jump()
{
	if (m_iFrame < 3)
	{
		m_Y -= m_velY;
	}
	else if (m_iFrame > 3)
	{
		m_Y += m_velY;

		if (m_Y > preJumpYVal)
			m_Y = preJumpYVal;
	}
}

void Enemy::OnJumpAnimationComplete()
{
	m_Y = preJumpYVal;
}

void Enemy::UpdateEnemy()
{
	m_pIsDefending = false;
	damagePoint = 0;

	if (playerDetected) 
	{
		Attack();
		Defense();
	}
	else
	{
		srand((unsigned)time(NULL));
		// % 4 meaning the default case will be more likely to happen then the other 2 states
		int randomNumber = (rand() % 4) + 0;

		switch (randomNumber) 
		{
			case 0: 
				RangedAttack();
				break;
			case 1:
				if (this->currentState != "Jump") preJumpYVal = m_Y;
				this->PlayState("Jump");
				break;
			default:
				MoveEnemy();
				break;
		}
		
	}

	if (currentState == "Jump") Jump();
	spriteSrcRect.x = spriteSrcRect.w * m_iFrame;
	this->UpdateDestRect();

	cout << LineOfSightCheck();

	
}

void Enemy::Attack() 
{
	m_pIsDefending = false;

	damagePoint = 0;
	srand((unsigned)time(NULL));
	int randomNumber = (rand() % (attackStates->size() - 1)) + 0;

	string currentAttack = attackStates[randomNumber];
	

	if (currentAttack.compare("Punch"))
	{
		damagePoint = 5;
	}
	else if (currentAttack.compare("Kick"))
	{
		damagePoint = 10;
	}
	else if (currentAttack.compare("Roundhouse"))
	{
		damagePoint = 15;
	}
	else // HADOUKEN ATTACK 
	{
		RangedAttack();
	}
	this->PlayState(currentAttack);
}

void Enemy::RangedAttack()
{
	if (LineOfSightCheck())
	{		
		
		this->PlayState("Hadouken");
		damagePoint = 30;
		cout << "HADOUKEN SHOUDA WORK!";
		// do damage here

	}
	else {
		MoveEnemy();
	}
	 // move to gain sight of player

	
}

void Enemy::Defense()
{
	srand((unsigned)time(NULL));
	int randomNumber = (rand() % (defenseStates->size() - 1)) + 0;

	switch (randomNumber)
	{
	case 0: 
		this->PlayState("Crouch");
		m_pIsDefending = true;
		break;
	case 1:
		if (this->currentState != "Jump") preJumpYVal = m_Y;
		this->PlayState("Jump");
		break;
	case 2:
		targetPositionX = m_X + m_dRadius + 10;
		MoveEnemy();
		break;
	default:
		// do nothing and take hit
		break;
	}
	if (currentState == "Jump") Jump();
}

void Enemy::CollisionCheck(bool isCollided)
{
	playerDetected = isCollided;
}

bool Enemy::LineOfSightCheck() 
{
	return (m_X - m_dRadius >= targetPositionX);
}


void Enemy::UpdateProjectiles()
{
	SDL_Window* sdlWindow = Game::Instance()->GetWindow();
	int win_width, win_height;
	SDL_GetWindowSize(sdlWindow, &win_width, &win_height);


	/*
	* OR another option - but it will get the entire screen size
	* SDL_DisplayMode DM;
	* SDL_GetCurrentDisplayMode(0, &DM);
	* auto screenWidth = DM.w;
	* auto screenHeight = DM.h;
	*/

	for (int i = 0; i < (int)m_vProjectiles.size(); i++)
	{
		m_vProjectiles[i]->Update();
		double tempPos = m_vProjectiles[i]->m_X;// old position

		if (m_vProjectiles[i]->m_X > win_width || m_vProjectiles[i]->m_Y > win_height)
		{
			DestroyProjectile(i);
		}
		else if (m_vProjectiles[i]->m_X > win_width * 0.9)
		{
			m_vProjectiles[i]->PlayState("Destroy");
			m_vProjectiles[i]->spriteSrcRect.x = m_vProjectiles[i]->spriteSrcRect.w * m_vProjectiles[i]->m_iFrame;
		}
		else
		{
			m_vProjectiles[i]->PlayState("Initialize");
			m_vProjectiles[i]->spriteSrcRect.x = m_vProjectiles[i]->spriteSrcRect.w * m_vProjectiles[i]->m_iFrame;

		}
	}

	m_vProjectiles.shrink_to_fit();
}


void Enemy::SpawnProjectiles()
{
	// Converting the player's angle to deltaX and deltaY
	float dx = (float)cos((angle)*M_PI / 180);
	float dy = (float)sin((angle)*M_PI / 180);

	m_vProjectiles.push_back(new Projectile(texture, m_X, m_Y, angle, dx, dy, -3,true));
}

void Enemy::DestroyProjectile(int i)
{
	//m_vProjectiles[i]->PlayState("Destroy");

	delete m_vProjectiles[i];
	m_vProjectiles[i] = nullptr;
	m_vProjectiles.erase(m_vProjectiles.begin() + i);
}

void Enemy::ScaleSpeedByLevel(double i)
{
	m_velX *= i;
}