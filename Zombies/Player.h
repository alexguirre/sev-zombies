#pragma once

#include "Actor.h"
#include "Weapon.h"
#include "Audio.h"
#include "Animation.h" // incluir animacion 

#define PLAYER_MAX_HEALTH 1000

class Player : public Actor
{
public:
	Player(float x, float y, Game* game);
	void update();
	void moveX(float axis);
	void moveY(float axis);
	void draw(float scrollX = 0, float scrollY = 0) override;
	void aimAt(float x, float y);
	void knifeAttack();
	void shoot();
	void receiveDamage(int damage);
	void switchWeapon();

	int health;
	int damageRedFlashTime;

	Weapon* weapon1;
	Weapon* weapon2;
	Weapon* currentWeapon;

	Animation* aKnifeAttack;
	Animation* animation;

	Animation* aFeetIdle;
	Animation* aFeetMove;
	Animation* aFeetMoveLeft;
	Animation* aFeetMoveRight;
	Animation* animationFeet;

	Audio* audioWeaponSwitch;
	Audio* audioDamage[4];
	int damagePlayAudioDelay;
};

