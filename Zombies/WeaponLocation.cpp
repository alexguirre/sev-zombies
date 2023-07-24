#include "WeaponLocation.h"
#include "GameLayer.h"
#include "Pistol.h"
#include "Shotgun.h"
#include "GrenadeLauncher.h"

static const char* getAnimationFilename(WeaponType type)
{
	switch (type)
	{
	default:
	case WeaponType::Pistol: return "res/level/pistol_location.png";
	case WeaponType::Shotgun: return "res/level/shotgun_location.png";
	case WeaponType::GrenadeLauncher: return "res/level/grenade_launcher_location.png";
	}
}

WeaponLocation::WeaponLocation(WeaponType type, int ammoRefill, int price, float x, float y, Game* game)
	: Actor("res/empty.png", x, y, 64, 60, game) {
	this->type = type;
	this->ammoRefill = ammoRefill;
	this->price = price;
	this->animation = new Animation(getAnimationFilename(type), 64, 60, 704, 60, 1, 11, true, game);
}

void WeaponLocation::draw(float scrollX, float scrollY) {
	animation->draw(x - scrollX, y - scrollY);
}

void WeaponLocation::update() {
	animation->update();
}

void WeaponLocation::buy(Player* player) {
	if (hasWeapon(player)) {
		// si el jugador ya tiene este arma, dar munición
		Weapon* weapon;
		if (player->weapon1 != nullptr && player->weapon1->type == type)
			weapon = player->weapon1;
		else
			weapon = player->weapon2;
		
		weapon->ammo += ammoRefill;
	}
	else {
		// si tiene un slot libre, dar el arma en ese slot
		// si no, reemplazar el arma que tiene seleccionada
		Weapon** weaponSlot;
		if (player->weapon1 == nullptr)
			weaponSlot = &player->weapon1;
		else if (player->weapon2 == nullptr)
			weaponSlot = &player->weapon2;
		else if (player->weapon1 == player->currentWeapon)
			weaponSlot = &player->weapon1;
		else
			weaponSlot = &player->weapon2;

		*weaponSlot = createWeapon();

		// seleccionar el nuevo arma
		if (player->currentWeapon != nullptr)
			player->currentWeapon->selected = false;
		player->currentWeapon = *weaponSlot;
		player->currentWeapon->selected = true;
		player->currentWeapon->ammo = ammoRefill;
	}

	game->gameLayer->audioBuy->stopAndPlay();
}

bool WeaponLocation::isActorInBuyRange(Actor* actor) {
	return distanceTo(actor) < 80;
}

bool WeaponLocation::hasWeapon(Player* player) {
	return (player->weapon1 != nullptr && player->weapon1->type == type) ||
		   (player->weapon2 != nullptr && player->weapon2->type == type);
}

Weapon* WeaponLocation::createWeapon() {
	switch (type)
	{
	case WeaponType::Pistol: return new Pistol(game);
	case WeaponType::Shotgun: return new Shotgun(game);
	case WeaponType::GrenadeLauncher: return new GrenadeLauncher(game);
	}

	return nullptr;
}

//PistolLocation::PistolLocation(float x, float y, Game* game)
//	: WeaponLocation(x, y, game) {
//}
//
//Weapon* PistolLocation::createWeapon() {
//	return new Pistol(game);
//}
//
//ShotgunLocation::ShotgunLocation(float x, float y, Game* game)
//	: WeaponLocation(x, y, game) {
//}
//
//Weapon* ShotgunLocation::createWeapon() {
//	return new Shotgun(game);
//}
//
//GrenadeLauncherLocation::GrenadeLauncherLocation(float x, float y, Game* game)
//	: WeaponLocation(x, y, game) {
//}
//
//Weapon* GrenadeLauncherLocation::createWeapon() {
//	return new GrenadeLauncher(game);
//}
