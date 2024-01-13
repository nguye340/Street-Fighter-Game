#include "Player.h"
#include "Game.h"
#include <algorithm> //for min/max functions

Player::Player(SDL_Texture *tex, double x, double y)
	: SpriteExAnimated(tex, x - 50, y - 50, 0, 1, 4, 0.1f)
{

	spriteSrcRect = { 0, 0, 70, 80 };
	spriteDestRect = { (int)(m_X - 50),(int) (m_Y-50), 70, 80};

	m_velX = 2;
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
		std::bind(&Player::OnJumpAnimationComplete, this));

}

Player::~Player()
{
	for (int i = 0; i < (int)m_vProjectiles.size(); i++)
	{
		delete m_vProjectiles[i];
		m_vProjectiles[i] = nullptr;
	}
	m_vProjectiles.clear();
	m_vProjectiles.shrink_to_fit();
}


void Player::Render()
{
	//rendering projectiles here
	for (int i = 0; i < (int)m_vProjectiles.size(); i++)
		m_vProjectiles[i]->Render();
	this->SpriteExAnimated::Render(); //invoke the base class's Render()

}

void Player::Update()
{
	this->UpdatePlayer();
	this->UpdateProjectiles();

}

void Player::Enable(bool isEnabledCheck)
{
	// disable when player about to go off-screen
	isEnabled = isEnabledCheck;
}


void Player::MovePlayer(bool isFwd)
{
	SDL_Rect bgDestRect;
	bgDestRect.x = bgDestRect.y = 0;
	SDL_GetWindowSize(Game::Instance()->GetWindow(), &bgDestRect.w, &bgDestRect.h);

	// make sure player can't go off-screen
	if (isEnabled)
	{
		if (isFwd)
			m_X += m_velX;
		else
			m_X -= m_velX;
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

	this->PlayState("Move");
}

void Player::Jump()
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

void Player::OnJumpAnimationComplete() 
{
	m_Y = preJumpYVal;
}

void Player::UpdatePlayer()
{
	damagePoint = 0;

	if (Game::Instance()->KeyDown(SDL_SCANCODE_D))
	{
		MovePlayer(true);
	}
	else if (Game::Instance()->KeyDown(SDL_SCANCODE_A)) 
	{
		MovePlayer(false);
	}
	else if (Game::Instance()->KeyDown(SDL_SCANCODE_SPACE))
	{
		if (this->currentState != "Jump") preJumpYVal = m_Y;
		this->PlayState("Jump");
	}
	else if (Game::Instance()->KeyDown(SDL_SCANCODE_P))
	{
		this->PlayState("Punch");
		damagePoint = 5;
	}
	else if (Game::Instance()->KeyDown(SDL_SCANCODE_F))
	{
		//play kick animation
		this->PlayState("Kick");
		damagePoint = 10;

	}
	else if (Game::Instance()->KeyDown(SDL_SCANCODE_R))
	{
		//to-do: play roundhouse animation
		this->PlayState("Roundhouse");
		damagePoint = 15;
	}
	
	else if (Game::Instance()->KeyDown(SDL_SCANCODE_Q) && this->GetIsFired())
	{
		//play hadouken animation
		this->PlayState("Hadouken");
		this->SpawnProjectiles();
		this->SetIsFired(false); // ensure that holding Q will not cast multiple projectile
		damagePoint = 30;
	}
	else // play idle animation
	{
		this->PlayState("Idle");
		spriteSrcRect.x = spriteSrcRect.w * m_iFrame;
	}

	if (currentState == "Jump") Jump();

	if (Game::Instance()->KeyUp(SDL_SCANCODE_Q))
		this->SetIsFired(true); // only releasing Q can player cast the next projectile

	spriteSrcRect.x = spriteSrcRect.w * m_iFrame;
	this->UpdateDestRect();

}

void Player::UpdateProjectiles()
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


void Player::SpawnProjectiles()
{
	// Converting the player's angle to deltaX and deltaY
	float dx = (float)cos((angle) * M_PI / 180);
	float dy = (float)sin((angle) * M_PI / 180);

	m_vProjectiles.push_back(new Projectile(texture, m_X, m_Y, angle, dx, dy, 3, false)); // 3 for speed - m_dSpeed
}

void Player::DestroyProjectile(int i)
{
	//m_vProjectiles[i]->PlayState("Destroy");

	delete m_vProjectiles[i];
	m_vProjectiles[i] = nullptr;
	m_vProjectiles.erase(m_vProjectiles.begin() + i);
}