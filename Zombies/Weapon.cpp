#include "Weapon.h"

Weapon::Weapon(WeaponType type, Game* game) {
	this->game = game;
	this->type = type;
	this->selected = false;
	this->ammo = 0;
	this->shootCadence = 0;
	this->shootTime = 0;
	this->audioShoot = nullptr;
	this->aHudIcon = nullptr;
}

void Weapon::update() {
	if (shootTime > 0) {
		shootTime--;
	}
}

bool Weapon::shoot(float startX, float startY, float rotation) {
	if (ammo <= 0 || shootTime > 0) {
		return false;
	}

	ammo--;
	shootTime = shootCadence;
	if (audioShoot) {
		audioShoot->play();
	}
	shootCreateProjectiles(startX, startY, rotation);
	return true;
}
