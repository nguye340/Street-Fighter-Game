#include "GameManager.h"
#include "Game.h"
#include "GameLevel1.h"
#include "GameLevel2.h"
#include "GameLevel3.h"
#include "VictoryScreen.h"

#include "ScreenState.h"

void GameManager::Update()
{
	if (_level == 4)
	{
		SetGameOver(true);
		if (_enemyScore < _playerScore)
		{
			SetGameStatus(true);
		}
		else if (_enemyScore > _playerScore)
		{
			SetGameStatus(false);
		}
		Game::Instance()->GetFSM()->ChangeState(new VictoryScreen());		
		_level++;
		_roundNum = 1;
	}

	UpdateScore();
	UpdateLevel();
}

void GameManager::UpdateLevel()
{
	// display you win/lose fonts

	// check and increment level

	if (_playerHealth <= 0 || _enemyHealth <= 0)
	{
		if (_roundNum == 3) // only when 3 rounds is finished can the player move the next level
		{
			_level++;
			_roundNum = 1;
		}
		else _roundNum++;

		//we need to switch to gamestate - level ... 
		switch (_level) {
		case 1:
			SetGameOver(false);
			ResetGameManager();
			Game::Instance()->GetFSM()->ChangeState(new GameLevel1());
			break;
		case 2:
			ResetGameManager();
			Game::Instance()->GetFSM()->ChangeState(new GameLevel2());
			break;
		case 3:
			ResetGameManager();
			Game::Instance()->GetFSM()->ChangeState(new GameLevel3());
			break;
		}
	}

}

void GameManager::UpdateScore()
{
	// check round win/lose status
	// display you win/lose/tie fonts	
	if (_playerHealth <= 0 || _enemyHealth <= 0)
	{

		if (_enemyHealth < _playerHealth)
		{
			_playerSubScore++;
		}
		else if (_playerHealth < _enemyHealth)
		{
			_enemySubScore++;
		}

		if (_roundNum == 3)
		{
			if (_enemySubScore < _playerSubScore)
			{
				_playerScore++;
			}
			else if (_enemySubScore > _playerSubScore)
			{
				_enemyScore++;
			}
		}
	}

}

bool GameManager::GetGameStatus()
{
	return 	_gameStatus;
};

void GameManager::SetGameStatus(bool isWinLose)
{
	_gameStatus = isWinLose;
};

void GameManager::ResetGameManager()
{
	_playerHealth = 100;
	_enemyHealth = 100;

	if (_level > 3)
	{
		_playerScore = 0;
		_enemyScore = 0;
		
		_playerSubScore = 0;
		_enemySubScore = 0;
		_level = 1;
	}
}

double GameManager::GetPlayerHealth()
{
	return _playerHealth;
};
double GameManager::GetEnemyHealth()
{
	return _enemyHealth;
};

void GameManager::PlayerTakeDamage(double amount)
{
	_playerHealth -= amount;
};
void GameManager::EnemyTakeDamage(double amount)
{
	_enemyHealth -= amount;
};


int GameManager::GetPlayerScore()
{
	return _playerScore;
};
int GameManager::GetEnemyScore()
{
	return _enemyScore;
};

int GameManager::GetPlayerSubScore()
{
	return _playerSubScore;
};

int GameManager::GetEnemySubScore()
{
	return _enemySubScore;
};

int GameManager::GetLevel()
{
	return _level;
};

int GameManager::GetRound()
{
	return _roundNum;
};

bool GameManager::CheckGameOver()
{
	if (_level > 3 && (_playerHealth <= 0 || _enemyHealth <= 0))
	{
		_isGameOver = true;
	}
	else _isGameOver = false;

	return _isGameOver;
};

void GameManager::SetGameOver(bool isOver)
{
	_isGameOver = isOver;
};

GameManager::~GameManager()
{
};
