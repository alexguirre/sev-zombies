#pragma once
#include "Game.h"
#include "Audio.h"
#include "Animation.h"

enum class WeaponType { Pistol, Shotgun, GrenadeLauncher };

class Weapon
{
public:
	Weapon(WeaponType type, Game* game);
	void update();
	bool shoot(float startX, float startY, float rotation);
	virtual void shootCreateProjectiles(float startX, float startY, float rotation) = 0;

	WeaponType type;
	bool selected;
	int ammo;
	int shootCadence;
	int shootTime;
	Animation* aPlayerIdle;
	Animation* aPlayerMove;
	Animation* aPlayerShoot;
	Audio* audioShoot;
	Animation* aHudIcon;
	Game* game;
};

