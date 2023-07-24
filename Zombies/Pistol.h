#pragma once
#include "Weapon.h"

class Pistol : public Weapon
{
public:
	Pistol(Game* game);
	void shootCreateProjectiles(float startX, float startY, float rotation) override;
};

