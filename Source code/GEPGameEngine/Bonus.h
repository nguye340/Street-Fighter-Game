#pragma once

#include "SpriteEx.h"
#include <vector>
using namespace std;

class Bonus : public SpriteExAnimated
{
private:

	double m_velX = 0,
		m_velY = 0,
		m_velMax = 10;
	int shieldHealth = 3;
	bool isBonus = false; // player's default no shield
	void UpdateBonus();

public:
	Bonus(SDL_Texture* tex, double x, double y);
	~Bonus();
	void Update();
	void SetIdle();
	void SetBonus(bool shieldEquippe);
	bool IsBonus();
	void SetSpriteX(double targetX); // x coord of player
	void SetSpriteY(double targetY); // y coord of player

	int GetBonusHealth();
	void decreaseBonusHealth();
};