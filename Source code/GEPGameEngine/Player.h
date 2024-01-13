#pragma once

#include "SpriteEx.h"
#include "Bullet.h"
#include <vector>
using namespace std;

class Player : public SpriteExAnimated
{
private:
	vector<Bullet*> m_vBullets;
	void UpdateBullets();

	double m_velX = 0,
		m_velY = 0,
		m_velMax = 10;
	bool isFired = true;
	float deltAngle = 0; // player' ship angle in relation to mouse

	void MoveForward();
	void MoveBackward();
	void MoveLeft();
	void MoveRight();
	void WrapAround();
	void UpdatePlayer();

public:
	Player(SDL_Texture* tex, double x, double y);
	~Player();
	void Update();
	void Render();
	void SetIdle();
	void SpawnBullet();

	void SetIsFired(bool alreadyFired) {
		isFired = alreadyFired;
	}
	bool GetIsFired() {
		return isFired;
	}

	vector<Bullet*>& GetBullets()
	{
		return m_vBullets;
	}
};