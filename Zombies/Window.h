#pragma once
#include "Actor.h"
#include "Animation.h"

#define WINDOW_BARRIER_MAX_HEALTH 3000

class Window : public Actor
{
public:
	Window(bool horizontal, float x, float y, Game* game);
	void update();
	void draw(float scrollX = 0, float scrollY = 0) override;
	bool isActorInRebuildBarrierRange(Actor* actor);
	void rebuildBarrier(int healthAmount);
	void damageBarrier(int healthAmount);
	bool canReachPlayer(); // se puede alcanzar al jugador desde esta ventana? (no hay puertas cerradas por en medio)

	int generateZombieTime;
	Animation* barrier;
	int barrierHealth;
	float insideDirX;
	float insideDirY;
};

