#include "Shotgun.h"
#include "Projectile.h"
#include "GameLayer.h"

Shotgun::Shotgun(Game* game) : Weapon(WeaponType::Shotgun, game) {
	shootCadence = 35;
	audioShoot = game->getAudio("res/audio/shotgun_shoot.wav", false);
	const float SCALE_FACTOR = 0.425f;
	aPlayerIdle = new Animation("res/player/shotgun_idle.png", 382 * SCALE_FACTOR, 155 * SCALE_FACTOR, 7640, 155, 3, 20, true, game);
	aPlayerMove = new Animation("res/player/shotgun_move.png", 383 * SCALE_FACTOR, 156 * SCALE_FACTOR, 7660, 156, 1, 20, true, game);
	aPlayerShoot = new Animation("res/player/shotgun_shoot.png", 380 * SCALE_FACTOR, 153 * SCALE_FACTOR, 1140, 153, 1, 3, false, game);
	aHudIcon = new Animation("res/hud/shotgun.png", 64, 16, 128, 16, 0, 2, false, game);
}

void Shotgun::shootCreateProjectiles(float startX, float startY, float rotation) {
	const float DAMAGE = 100;
	const float SPEED = 30.0f;
	const int NUM_BULLETS = 5;
	for (int i = 0; i < NUM_BULLETS; i++) {
		float randomRot = rand() % 15 * (rand() & 1 ? -1.0f : 1.0f);
		float dirX, dirY;
		Game::getDirectionFromRotation(rotation + randomRot, dirX, dirY);
		game->gameLayer->createProjectile(startX, startY, dirX * SPEED, dirY * SPEED, DAMAGE, 5, 150, true, false);
	}
}
