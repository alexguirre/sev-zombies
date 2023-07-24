#pragma once
#include "Actor.h"
#include "Weapon.h"
#include "Player.h"

class WeaponLocation : public Actor
{
public:
	WeaponLocation(WeaponType type, int ammoRefill, int price, float x, float y, Game* game);
	void draw(float scrollX = 0, float scrollY = 0) override;
	void update();
	void buy(Player* player);
	bool isActorInBuyRange(Actor* actor);
	bool hasWeapon(Player* player);
	Weapon* createWeapon();

	Animation* animation;
	WeaponType type;
	int ammoRefill;
	int price;
};
