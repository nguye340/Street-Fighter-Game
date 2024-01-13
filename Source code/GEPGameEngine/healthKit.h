#pragma once

#include "SpriteEx.h"
#include <vector>
using namespace std;

class HealthKit : public SpriteExAnimated
{
private:

	double m_velX = 0,
		m_velY = 0,
		m_velMax = 10;
	int shieldHealth = 3;
	bool isHealthKit = false; // player's default no shield
	void UpdateHealthKit();

public:
	HealthKit(SDL_Texture* tex, double x, double y);
	~HealthKit();
	void Update();	
};