#pragma once
#include "SpriteEx.h"
using namespace std;

class Projectile : public SpriteExAnimated {

public:
	friend class Player;
	friend class Enemy;
	bool isDestroyed = false;

	Projectile(SDL_Texture* tex, double x, double y, double a, double dx, double dy, double speed, bool isEnemyProjectile) : SpriteExAnimated(tex, x, y, a, 0, 0, 20.0f)
	{
		m_DX = dx;
		m_DY = dy;
		m_dRadius = 8;
		m_dSpeed = speed;
		isFlipped = isEnemyProjectile;
		spriteSrcRect = { 0, 320, 70, 80 };
		spriteDestRect = { (int)(x - 12),(int)(y - 12),70, 80 };

		AddAnimState("Initialize", AnimStateDefinition(4, 2, 120, true));
		AddAnimState("Destroy", AnimStateDefinition(5, 4, 120));

		animStates["Destroy"].RegisterOnAnimCompleteCallback(
			std::bind(&Projectile::OnDestroyProjectileAnimComplete, this));

		isDestroyed = false;
	}

	void Update()
	{
		m_X += (m_DX * m_dSpeed);
		m_Y += (m_DY * m_dSpeed);

		this->UpdateDestRect();
	}
	void OnDestroyProjectileAnimComplete()
	{
		isDestroyed = true;
	};
};