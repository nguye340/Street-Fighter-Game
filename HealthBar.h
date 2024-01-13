#pragma once
#include "SpriteEx.h"
#include "SDL.h"
#include <vector>
using namespace std;

class HealthBar : public SpriteExAnimated
{
private:

	//SDL_Texture* healthBarBgTex;

	float maxValue = 100.0f; // default max value 100
	float currentValue = 0.0f;
	bool isMaxed = false;

	//SDL_Rect bgSrcRect;

public:
	HealthBar(SDL_Texture* tex, double x, double y, bool isEnemyHealthBar);
	HealthBar* healthBarFill;

	~HealthBar();
	void UpdateCurrentValue(float value);
	void SetMaxValue(float maxVal);
	void OnHealthBarComplete();
	void Render();
	bool IsMaxedOut();

	//setter
	void setCompleteState(bool completeCheck)
	{
		isMaxed = completeCheck;
	}
};