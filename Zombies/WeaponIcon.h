#pragma once
#include "Actor.h"
#include "Text.h"
#include "Animation.h"
#include "Weapon.h"

class WeaponIcon : public Actor
{
public:
	WeaponIcon(float x, float y, Weapon* weapon, Game* game);
	virtual void draw(float scrollX = 0, float scrollY = 0);

	Weapon* weapon;
	Text* ammoText;
};

