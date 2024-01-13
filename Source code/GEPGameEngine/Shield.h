#pragma once

#include "SpriteEx.h"
#include <vector>
using namespace std;

class Shield : public SpriteExAnimated
{
private:

	double m_velX = 0,
		m_velY = 0,
		m_velMax = 10;
	int shieldHealth = 3;
	bool isShield = false; // player's default no shield
	void UpdateShield();

public:
	Shield(SDL_Texture* tex, double x, double y);
	~Shield();
	void Update();
	void SetIdle();
	void SetShield(bool shieldEquippe);
	bool IsShield();
	void SetSpriteX(double targetX); // x coord of player
	void SetSpriteY(double targetY); // y coord of player

	int GetShieldHealth();
	void decreaseShieldHealth();
};