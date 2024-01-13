#pragma once

#include "SpriteEx.h"
#include "Bullet.h"
#include <vector>
using namespace std;

class Enemy : public SpriteExAnimated
{
private:
	vector<Bullet*> m_vBullets;
	void UpdateBullets();

	double m_velX = 0,
		m_velY = 0,
		m_velMax = 10;
	bool isFired = true;
	float rotationSpeed = 2;

	void WrapAround();
	void MoveForward();
	void MoveLeft();
	void MoveRight();
	void UpdateEnemy();

public:
	Enemy(SDL_Texture* tex, double x, double y);
	~Enemy();
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