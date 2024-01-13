#pragma once
#include <iostream>
using namespace std;

class GameManager
{
protected:
	double _playerHealth = 100;
	double _enemyHealth = 100;

	int _playerScore = 0;
	int _enemyScore = 0;
	int _playerSubScore = 0;
	int _enemySubScore = 0;

	int _level = 1;
	int _roundNum = 1;

	bool _gameStatus = true;
	bool _isGameOver = false;

public:

	~GameManager();

	void Update();
	void Render();
	void UpdateLevel();
	void UpdateScore();
	void ResetGameManager();

	double GetPlayerHealth();
	double GetEnemyHealth();

	int GetPlayerScore();
	int GetEnemyScore();

	int GetPlayerSubScore();
	int GetEnemySubScore();

	int GetLevel();
	int GetRound();

	void PlayerTakeDamage(double amount);
	void EnemyTakeDamage(double amount);

	bool GetGameStatus(); // for each round

	bool CheckGameOver(); // for all three levels
	void SetGameStatus(bool isWinLose);
	void SetGameOver(bool isOver); // for all three levels

};
