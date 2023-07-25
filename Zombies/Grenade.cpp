#include "Grenade.h"
#include "GameLayer.h"

Grenade::Grenade(float x, float y, float vx, float vy, int damage, int timeToExplode, Game* game) :
	Actor("res/grenade.png", x, y, 28, 14, game) {
	this->vx = vx;
	this->vy = vy;
	this->damage = damage;
	this->timeToExplode = timeToExplode;
	this->dynamicActorCollidesWith = COLLISION_ALL & ~COLLISION_WINDOW; // colisiona con todo excepto ventanas

	bounce = true;
	bounciness = 0.15f;

	aExplosion = new Animation("res/grenade_explosion.png", 384, 384, 12 * 384, 384, 1, 12, false, game);
	audioExplosion = game->getAudio("res/audio/grenade_explosion.wav", false);

	shouldBeDeleted = false;
}

void Grenade::draw(float scrollX, float scrollY) {
	if (timeToExplode > 0) {
		if (vx != 0.0f || vy != 0.0f) {
			rotation = Game::getRotationFromDirection(vx, vy);
		}
		Actor::draw(scrollX, scrollY);
	}
	else {
		aExplosion->draw(x - scrollX, y - scrollY);
	}
}

void Grenade::update() {
	if (timeToExplode > 0) {
		timeToExplode--;

		// aplicar fricción
		const float FRICTION = 0.125f;
		if (vx < 0.0f) vx = min(0.0f, vx + FRICTION);
		if (vx > 0.0f) vx = max(0.0f, vx - FRICTION);
		if (vy < 0.0f) vy = min(0.0f, vy + FRICTION);
		if (vy > 0.0f) vy = max(0.0f, vy - FRICTION);

		if (timeToExplode <= 0) {
			vx = 0;
			vy = 0;
			audioExplosion->play();
			game->gameLayer->shakeCamera();

			// aplicar daño al jugador y a los zombies cercanos
			if (isActorInExplosionRange(game->gameLayer->player)) {
				game->gameLayer->player->receiveDamage(damage);
			}

			for (auto zombie : game->gameLayer->zombies) {
				if (isActorInExplosionRange(zombie)) {
					zombie->receiveDamage(damage);
					game->gameLayer->addScore(50, zombie->x, zombie->y);
				}
			}
		}
	}
	else {
		bool animationEnded = aExplosion->update();
		shouldBeDeleted = animationEnded;
	}
}

bool Grenade::isActorInExplosionRange(Actor* actor) {
	return distanceTo(actor) < 200;
}
