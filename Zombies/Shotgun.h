#pragma once
#include "Weapon.h"

class Shotgun : public Weapon
{
public:
	Shotgun(Game* game);
	void shootCreateProjectiles(float startX, float startY, float rotation) override;
};

