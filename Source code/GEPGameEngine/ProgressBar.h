#pragma once
#include "SpriteEx.h"
#include "SDL.h"
#include <vector>
using namespace std;

class ProgressBar : public SpriteExAnimated
{
private:

	//SDL_Texture* progressBarBgTex;

	float maxValue = 100.0f; // default max value 100
	float currentValue = 0.0f;
	bool isMaxed = false;

	//SDL_Rect bgSrcRect;

public:
	ProgressBar(SDL_Texture* tex, double x, double y);
	ProgressBar* progressBarFill;

	~ProgressBar();
	void UpdateCurrentValue(float value);
	void SetMaxValue(float maxVal);
	void OnProgressBarComplete();
	void Render();
	bool IsMaxedOut();

	//setter
	void setCompleteState(bool completeCheck)
	{
		isMaxed = completeCheck;
	}
};

