#include "Player.h"
#include "Game.h"
#include <algorithm> //for min/max function

Player::Player(SDL_Texture* tex, double x, double y)
	: SpriteExAnimated(tex, x - 50, y - 50, 0, 1, 4, 0.1f)
{

	spriteSrcRect = { 0,0,100,100 };
	spriteDestRect = { (int)(m_X - 50),(int)(m_Y - 50)  ,100,100 };

	m_dRadius = 50;
}

Player::~Player()
{
	for (int i = 0; i < (int)m_vBullets.size(); i++)
	{
		delete m_vBullets[i];
		m_vBullets[i] = nullptr;
	}
	m_vBullets.clear();
	m_vBullets.shrink_to_fit();

}


void Player::Render()
{
	//rendering bullets here
	for (int i = 0; i < (int)m_vBullets.size(); i++)
		m_vBullets[i]->Render();

	this->SpriteExAnimated::Render(); //invoke the base class's Render()

}


void Player::SetIdle()
{
	spriteSrcRect.y = 0;
	m_iFrame = 0;
	m_iSprite = 0;
}

void Player::Update()
{
	//implement a rotation mechanism keyboar and mouse (improvement)
	if (Game::Instance()->KeyDown(SDL_SCANCODE_Q) || Game::Instance()->GetLeftMouse())
		angle -= 6.0;
	else if (Game::Instance()->KeyDown(SDL_SCANCODE_E) || Game::Instance()->GetRightMouse())
		angle += 6.0;

	this->UpdatePlayer();
	this->UpdateBullets();

}

void Player::MoveForward()
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

void Player::MoveBackward()
{
	// Convert player spaceship's angle to deltaX and deltaY
	m_DX = (float)cos((angle - 90) * M_PI / 180);
	m_DY = (float)sin((angle - 90) * M_PI / 180);

	// Update the velocities
	m_velX -= m_DX * m_dSpeed;
	m_velY -= m_DY * m_dSpeed;

	// Clam the velocities and maintain previous vecter i.e dx and dy
	m_velX = min(max(m_velX, -(m_velMax * fabs(m_DX))), (m_velMax * fabs(m_DX)));
	m_velY = min(max(m_velY, -(m_velMax * fabs(m_DY))), (m_velMax * fabs(m_DY)));
}

void Player::MoveLeft()
{
	// Convert player spaceship's angle to deltaX and deltaY
	m_DX = (float)cos((angle)*M_PI / 180);
	m_DY = (float)sin((angle)*M_PI / 180);

	// Update the velocities
	m_velX -= m_DX * m_dSpeed;
	m_velY -= m_DY * m_dSpeed;

	// Clam the velocities and maintain previous vecter i.e dx and dy
	m_velX = min(max(m_velX, -(m_velMax * fabs(m_DX))), (m_velMax * fabs(m_DX)));
	m_velY = min(max(m_velY, -(m_velMax * fabs(m_DY))), (m_velMax * fabs(m_DY)));
}

void Player::MoveRight()
{
	// Convert player spaceship's angle to deltaX and deltaY
	m_DX = (float)cos((angle)*M_PI / 180);
	m_DY = (float)sin((angle)*M_PI / 180);

	// Update the velocities
	m_velX = m_DX * 0.5 / m_dSpeed;
	m_velY = m_DY * 0.5 / m_dSpeed;

	// Clam the velocities and maintain previous vecter i.e dx and dy
	m_velX = min(max(m_velX, -(m_velMax * fabs(m_DX))), (m_velMax * fabs(m_DX)));
	m_velY = min(max(m_velY, -(m_velMax * fabs(m_DY))), (m_velMax * fabs(m_DY)));
}


void Player::UpdatePlayer()
{
	if (Game::Instance()->KeyDown(SDL_SCANCODE_W))
	{
		spriteSrcRect.y = 100;//set y to the 100 so that we start playing the 2nd row
		this->Animate();
		this->MoveForward();

	}
	else if (Game::Instance()->KeyDown(SDL_SCANCODE_S))
		this->MoveBackward();
	else if (Game::Instance()->KeyDown(SDL_SCANCODE_A))
		this->MoveLeft();
	else if (Game::Instance()->KeyDown(SDL_SCANCODE_D))
		this->MoveRight();
	else
	{
		// add some drag to slow down
		m_velX *= 0.95;
		m_velY *= 0.95;
	}

	m_X += m_velX; // update x coord
	m_Y += m_velY; // update y coord

	this->WrapAround();
	// wraparound:
	this->UpdateDestRect();
	spriteSrcRect.x = spriteSrcRect.w * m_iSprite;// update the animation
}

void Player::UpdateBullets()
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


void Player::SpawnBullet()
{

	// Converting the player spaceship's angle to deltaX and deltaY
	float dx = (float)cos((angle - 90) * M_PI / 180);
	float dy = (float)sin((angle - 90) * M_PI / 180);

	m_vBullets.push_back(new Bullet(texture, m_X, m_Y, angle, dx, dy));
}

void Player::WrapAround()
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
