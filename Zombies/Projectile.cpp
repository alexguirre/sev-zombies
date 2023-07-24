#include "Projectile.h"
#include "GameLayer.h"

Projectile::Projectile(float x, float y, float vx, float vy, int damage, int hitScore, int timeToLive, bool isBullet, bool fromZombie, Game* game) :
	Actor("res/empty.png", x, y, 5, 5, game) {
	this->vx = vx;
	this->vy = vy;
	this->initialVX = vx;
	this->initialVY = vy;
	this->shouldBeDeleted = false;
	this->damage = damage;
	this->hitScore = hitScore;
	this->timeToLive = timeToLive;
	this->fromZombie = fromZombie;
	this->dynamicActorCollidesWith = COLLISION_ALL & ~COLLISION_WINDOW; // colisiona con todo excepto ventanas
	if (isBullet) {
		this->aBulletTrace = new Animation("res/bullet_trace.png", 24, 3, 120, 3, 1, 5, true, game);
		this->aBulletTrace->currentFrame = rand() % aBulletTrace->totalFrames;
	}
	else {
		this->aBulletTrace = nullptr;
	}
}

void Projectile::draw(float scrollX, float scrollY) {
	if (aBulletTrace) {
		if (vx != 0.0f || vy != 0.0f) {
			rotation = Game::getRotationFromDirection(vx, vy);
		}
		aBulletTrace->draw(x - scrollX, y - scrollY, rotation);
	}
}

void Projectile::update() {
	shouldBeDeleted = timeToLive <= 0 || vx != initialVX || vy != initialVY;
	timeToLive--;
	if (aBulletTrace) {
		aBulletTrace->update();
	}

	// Comprobar colisiones con otros actores
	if (fromZombie) {
		// Un proyectil de un zombie, comprobar colisiones con el jugador y las ventanas
		if (isOverlap(game->gameLayer->player)) {
			game->gameLayer->player->receiveDamage(damage);
			shouldBeDeleted = true;
		}

		for (auto window : game->gameLayer->windows) {
			if (isOverlap(window)) {
				window->damageBarrier(damage);
				shouldBeDeleted = true;
			}
		}
	}
	else {
		// Un proyectil del jugador, comprobar colisiones con los zombies
		for (auto zombie : game->gameLayer->zombies) {
			if (isOverlap(zombie)) {
				zombie->receiveDamage(damage);
				game->gameLayer->addScore(hitScore, x, y);
				shouldBeDeleted = true;
			}
		}
	}
}
