#pragma once

#include "Actor.h"
#include "Animation.h"

class Projectile : public Actor
{
public:
	Projectile(float x, float y, float vx, float vy, int damage, int hitScore, int timeToLive, bool isBullet, bool fromZombie, Game* game);
	void draw(float scrollX = 0, float scrollY = 0) override;
	void update();

	Animation* aBulletTrace;
	float initialVX;
	float initialVY;
	bool shouldBeDeleted;
	int damage;
	int hitScore;
	int timeToLive;
	bool fromZombie;
};

