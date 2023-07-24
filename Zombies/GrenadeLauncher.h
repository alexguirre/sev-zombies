#pragma once
#include "Weapon.h"

class GrenadeLauncher : public Weapon
{
public:
	GrenadeLauncher(Game* game);
	void shootCreateProjectiles(float startX, float startY, float rotation) override;
};

