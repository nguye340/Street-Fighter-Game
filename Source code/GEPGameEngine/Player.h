#pragma once
#include "Projectile.h"
#include "SpriteEx.h"
#include <vector>
using namespace std;


class Player : public SpriteExAnimated
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
	bool isFired = true;

	double damagePoint = 0;

	void MovePlayer(bool isFwd);
	void UpdatePlayer();
	void Jump();
	void OnJumpAnimationComplete();

public :
	Player(SDL_Texture *tex, double x, double y);
	~Player();
	void Update();
	void Render();
	void Enable(bool isEnabledCheck); // False if player is off-screen

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

	double GetDamagePoint() {
		return damagePoint;
	}

	void SetDamagePoint(double amount) {
		damagePoint = amount;
	}

};