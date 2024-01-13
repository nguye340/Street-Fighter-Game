#include "Enemy.h"
#include "Game.h"
#include <algorithm> //for min/max function

Enemy::Enemy(SDL_Texture* tex, double x, double y)
	: SpriteExAnimated(tex, x - 50, y - 160, 0, 1, 5, 0.001f)
{

	spriteSrcRect = { 140,0,140,356};
	spriteDestRect = { (int)(m_X - 50),(int)(m_Y - 160), 50, 160 };

	m_dRadius = 60;

	rotationSpeed = (1 - rand() % 2 * 2) * (rand() % 6 + 1);
}

Enemy::~Enemy()
{
	for (int i = 0; i < (int)m_vBullets.size(); i++)
	{
		delete m_vBullets[i];
		m_vBullets[i] = nullptr;
	}
	m_vBullets.clear();
	m_vBullets.shrink_to_fit();

}


void Enemy::Render()
{
	//rendering bullets here
	for (int i = 0; i < (int)m_vBullets.size(); i++)
		m_vBullets[i]->Render();

	this->SpriteExAnimated::Render(); //invoke the base class's Render()

}


void Enemy::SetIdle()
{
	spriteSrcRect.y = 0;
	m_iFrame = 0;
	m_iSprite = 0;
}

void Enemy::MoveForward()
{
	// Convert player spaceship's angle to deltaX and deltaY
	m_DX = (float)cos((angle - 90) * M_PI / 180);
	m_DY = (float)sin((angle - 90) * M_PI / 180);

	// Update the velocities
	m_velX += m_DX * m_dSpeed;
	m_velY += m_DY * m_dSpeed;

	// Clam the velocities and maintain previous vecter i.e dx and dy
	m_velX = min(max(m_velX, -(m_velMax * fabs(m_DX))), (m_velMax * fabs(m_DX)));
	m_velY = min(max(m_velY, -(m_velMax * fabs(m_DY))), (m_velMax * fabs(m_DY)));
}


void Enemy::MoveLeft()
{
	// Convert player spaceship's angle to deltaX and deltaY
	m_DX = (float)cos((angle)*M_PI / 180);
	m_DY = (float)sin((angle)*M_PI / 180);

	// Update the velocities
	m_velX -= m_DX * 0.2 / m_dSpeed;
	m_velY -= m_DY * 0.2 / m_dSpeed;

	// Clam the velocities and maintain previous vecter i.e dx and dy
	m_velX = min(max(m_velX, -(m_velMax * fabs(m_DX))), (m_velMax * fabs(m_DX)));
	m_velY = min(max(m_velY, -(m_velMax * fabs(m_DY))), (m_velMax * fabs(m_DY)));
}

void Enemy::MoveRight()
{
	// Convert player spaceship's angle to deltaX and deltaY
	m_DX = (float)cos((angle)*M_PI / 180);
	m_DY = (float)sin((angle)*M_PI / 180);

	// Update the velocities
	m_velX = m_DX * 0.2 / m_dSpeed;
	m_velY = m_DY * 0.2 / m_dSpeed;

	// Clam the velocities and maintain previous vecter i.e dx and dy
	m_velX = min(max(m_velX, -(m_velMax * fabs(m_DX))), (m_velMax * fabs(m_DX)));
	m_velY = min(max(m_velY, -(m_velMax * fabs(m_DY))), (m_velMax * fabs(m_DY)));
}


void Enemy::Update()
{
	//implement a rotation mechanism
	//angle += rotationSpeed;
	this->UpdateEnemy();
	this->UpdateBullets();

}


void Enemy::UpdateEnemy()
{
	angle = 180;
	this->Animate();

	// Enemy will match player's movement to track player
	if (Game::Instance()->KeyDown(SDL_SCANCODE_A))
		this->MoveRight();
	else if (Game::Instance()->KeyDown(SDL_SCANCODE_D))
		this->MoveLeft();
	else
	{
		this->MoveForward();
		m_X += m_velX * 5; // update x coord
		m_Y += m_velY * 5; // update y coord
	}

	m_X += m_velX; // update x coord
	m_Y += m_velY; // update y coord
	
	this->WrapAround();
	// wraparound:
	this->UpdateDestRect();
	spriteSrcRect.x = spriteSrcRect.w * m_iSprite;// update the animation
}

void Enemy::UpdateBullets()
{
	SDL_Window* sdlWindow = Game::Instance()->GetWindow();
	int win_width, win_height;
	SDL_GetWindowSize(sdlWindow, &win_width, &win_height);


	for (int i = 0; i < (int)m_vBullets.size(); i++)
	{
		m_vBullets[i]->Update();

		if (m_vBullets[i]->m_X < 0 || m_vBullets[i]->m_X > win_width
			|| m_vBullets[i]->m_Y < 0 || m_vBullets[i]->m_Y > win_height)
		{
			delete m_vBullets[i];
			m_vBullets[i] = nullptr;
			m_vBullets.erase(m_vBullets.begin() + i);

		}
	}

	m_vBullets.shrink_to_fit();
}


void Enemy::SpawnBullet()
{

	// Converting the enemy spaceship's angle to deltaX and deltaY
	float dx = (float)cos((angle - 90) * M_PI / 180);
	float dy = (float)sin((angle - 90) * M_PI / 180);

	m_vBullets.push_back(new Bullet(texture, m_X, m_Y, angle, dx, dy));

}

void Enemy::WrapAround()
{
	SDL_Window* sdlWindow = Game::Instance()->GetWindow();
	int win_width, win_height;
	SDL_GetWindowSize(sdlWindow, &win_width, &win_height);


	if (m_X < 0) {
		m_X = win_width;
	}
	if (m_X > win_width) {
		m_X = 0;
	}
	if (m_Y < 0) {
		m_Y = win_height;
	}
	if (m_Y > win_height) {
		m_Y = 0;
	}
}

