#include "Pistol.h"
#include "Projectile.h"
#include "GameLayer.h"

Pistol::Pistol(Game* game) : Weapon(WeaponType::Pistol, game) {
	shootCadence = 9;
	audioShoot = game->getAudio("res/audio/pistol_shoot.wav", false);
	const float SCALE_FACTOR = 0.425f;
	aPlayerIdle = new Animation("res/player/pistol_idle.png", 253 * SCALE_FACTOR, 216 * SCALE_FACTOR, 5060, 216, 3, 20, true, game);
	aPlayerMove = new Animation("res/player/pistol_move.png", 258 * SCALE_FACTOR, 220 * SCALE_FACTOR, 5160, 220, 1, 20, true, game);
	aPlayerShoot = new Animation("res/player/pistol_shoot.png", 255 * SCALE_FACTOR, 215 * SCALE_FACTOR, 765, 215, 1, 3, false, game);
	aHudIcon = new Animation("res/hud/pistol.png", 48, 32, 96, 32, 0, 2, false, game);
}

void Pistol::shootCreateProjectiles(float startX, float startY, float rotation) {
	const float DAMAGE = 250;
	const float SPEED = 40.0f;
	float dirX, dirY;
	Game::getDirectionFromRotation(rotation, dirX, dirY);
	game->gameLayer->createProjectile(startX, startY, dirX * SPEED, dirY * SPEED, DAMAGE, 20, 150, true, false);
}
