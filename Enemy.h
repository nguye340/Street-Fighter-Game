#pragma once

#include "Projectile.h"
#include "SpriteEx.h"
#include <vector>

using namespace std;

class Enemy : public SpriteExAnimated
{
private:
	vector<Projectile*> m_vProjectiles;
	void UpdateProjectiles();
	void SpawnProjectiles();

	double m_velX = 0,
		m_velY = 0,
		m_velMax = 10;
	double preJumpYVal;
	bool isEnabled = true;
	bool playerDetected = false;
	double targetPositionX;

	bool isFired = true;
	bool m_pIsDefending = false;
	double damagePoint = 0;

	// array with fixed-sized provide better performance in for-loop,
	// other classes cannot add new states and arr index useful in randomization 
	string attackStates[4] = {"Punch","Kick","Roundhouse","Hadouken"};
	string defenseStates[4] = {"Crouch","Jump","Move", "TakeHit"};
	
	void UpdateEnemy();
	void MoveEnemy();
	
	void Attack();
	void RangedAttack();

	void Defense();

	void OnJumpAnimationComplete();

public:
	Enemy(SDL_Texture* tex, double x, double y);
	~Enemy();

	bool isMoving = true;
	void Jump();

	void Update();
	void Render();
	void Enable(bool isEnabledCheck); // False if enemy is off-screen
	void CollisionCheck(bool isCollided);
	void SetTargetPositionX(double playerPos);
	bool LineOfSightCheck(); // check if collision/player is within line of sight
	void ScaleSpeedByLevel(double i);

	void DestroyProjectile(int i);

	void SetIsFired(bool alreadyFired) {
		isFired = alreadyFired;
	}
	bool GetIsFired() {
		return isFired;
	}

	vector<Projectile*> GetProjectiles()
	{
		return m_vProjectiles;
	}

	bool IsDefending() {
		return m_pIsDefending;
	}

	double GetDamagePoint() {
		return damagePoint;
	}

	void SetDamagePoint(double amount) {
		damagePoint = amount;
	}
};	
