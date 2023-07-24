#pragma once

#include "Actor.h"
#include "Animation.h"
#include "Audio.h"

class Grenade : public Actor
{
public:
	Grenade(float x, float y, float vx, float vy, int damage, int timeToExplode, Game* game);
	void draw(float scrollX = 0, float scrollY = 0) override;
	void update();
	bool isActorInExplosionRange(Actor* actor);

	Animation* aExplosion;
	Audio* audioExplosion;
	bool shouldBeDeleted;
	int damage;
	int timeToExplode;
};

