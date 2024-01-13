#pragma once
#include "ScreenState.h"
#include <iostream>

#include "Asteroid.h"
#include "Enemy.h"

#include "Shield.h"
#include "HealthKit.h"

#include "Player.h"
#include <SDL_mixer.h>
#include "Bonus.h"
using namespace std;

class GameState : public ScreenState
{
private:
	SDL_Texture* mainSpriteTex;//load and store the main sprite texture
	SDL_Texture* bgSpriteTex; //load and store background sprite tex
	SDL_Texture* enemySpriteTex; //load and store enemy ship tex
	SDL_Texture* shieldSpriteTex; //load and store shield tex
	SDL_Texture* bonusSpriteTex; //load and store shield tex
	

	SpriteEx* bg;

	Player* player;
	Shield* shield;
	vector<Asteroid*> asteroids;
	vector<Enemy*> enemies;
	vector<HealthKit*> healthKits;
	vector<Bonus*> bonuses;


	Mix_Chunk* fireSound;
	Mix_Chunk* engineSound;

	int lives = 3;
	int score = 0;
	int scoreMulti = 1;// if pick up bonus, score will multiply accordingly


public:
	GameState() {}
	void Update();
	void Render();
	void Enter();
	void Exit();
	void Resume() { cout << "Resuming Game..." << endl; }
	bool CircleCollisionTest(double x1, double y1,
		double x2, double y2, double r1, double r2)
	{
		return (sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)) < (r1 + r2));
	}
	void CheckCollision();
	int GetScore() 
	{
		return score;
	};
	void CheckEndGame();

};

