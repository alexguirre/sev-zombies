#pragma once

#include "Actor.h"
#include "Animation.h" 

class Window;

class Zombie : public Actor
{
public:
	Zombie(float x, float y, Game* game);
	void draw(float scrollX = 0, float scrollY = 0) override;
	void update();
	void moveTo(float targetX, float targetY);
	void receiveDamage(int damage);
	void attack();

	float vxIntelligence;
	float vyIntelligence;
	float debugTargetXIntelligence;
	float debugTargetYIntelligence;
	Window* targetWindow;

	int health;
	int damageRedFlashTime;
	Animation* aIdle;
	Animation* aMove;
	Animation* aAttack;
	Animation* animation;
};
