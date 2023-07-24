#include "GrenadeLauncher.h"
#include "Grenade.h"
#include "GameLayer.h"

GrenadeLauncher::GrenadeLauncher(Game* game) : Weapon(WeaponType::GrenadeLauncher, game) {
	shootCadence = 40;
	audioShoot = game->getAudio("res/audio/grenade_launcher_shoot.wav", false);
	const float SCALE_FACTOR = 0.425f;
	aPlayerIdle = new Animation("res/player/pistol_idle.png", 253 * SCALE_FACTOR, 216 * SCALE_FACTOR, 5060, 216, 3, 20, true, game);
	aPlayerMove = new Animation("res/player/pistol_move.png", 258 * SCALE_FACTOR, 220 * SCALE_FACTOR, 5160, 220, 1, 20, true, game);
	aPlayerShoot = new Animation("res/player/pistol_shoot.png", 255 * SCALE_FACTOR, 215 * SCALE_FACTOR, 765, 215, 1, 3, false, game);
	aHudIcon = new Animation("res/hud/grenade_launcher.png", 63, 20, 126, 20, 0, 2, false, game);
}

void GrenadeLauncher::shootCreateProjectiles(float startX, float startY, float rotation) {
	const float DAMAGE = 500;
	const float SPEED = 50.0f;
	float dirX, dirY;
	Game::getDirectionFromRotation(rotation, dirX, dirY);
	game->gameLayer->createGrenade(startX, startY, dirX * SPEED, dirY * SPEED, DAMAGE, 75);
}
