#include "Window.h"
#include "Zombie.h"
#include "GameLayer.h"

Window::Window(bool horizontal, float x, float y, Game* game)
	: Actor(horizontal ? "res/level/window_horizontal.png" : "res/level/window_vertical.png", x, y, horizontal ? 128 : 64, horizontal ? 64 : 128, game) {
	barrier = new Animation(horizontal ? "res/level/barrier_horizontal.png" : "res/level/barrier_vertical.png", width, height, horizontal ? 512 : 256, horizontal ? 64 : 128, 0, 4, true, game);
	barrierHealth = WINDOW_BARRIER_MAX_HEALTH;
	staticActorCollisionFlags = COLLISION_WINDOW;
	generateZombieTime = 100 + rand() % 50;

	// decidir en que dirección desde esta venta se encuentra el interior del nivel
	if (horizontal) {
		insideDirX = 0;
		insideDirY = y <= TILE_HEIGHT ? 1 : -1;
	}
	else { // vertical
		insideDirX = x <= TILE_WIDTH ? 1 : -1;
		insideDirY = 0;
	}
}

void Window::update() {
	// Solo generar zombies si se puede alcanzar al jugador desde esta ventana.
	// Puede haber puertas que bloqueen el camino hasta que las abra el jugador
	bool canGenerateZombies = canReachPlayer();

	if (canGenerateZombies) {
		generateZombieTime--;

		if (generateZombieTime <= 0) {
			float outsideDirX = -insideDirX;
			float outsideDirY = -insideDirY;
			float randomRot = rand() % 40 * (rand() & 1 ? -1.0f : 1.0f);
			float outsideRot = Game::getRotationFromDirection(outsideDirX, outsideDirY);
			Game::getDirectionFromRotation(outsideRot + randomRot, outsideDirX, outsideDirY);

			game->gameLayer->createZombie(x + outsideDirX * 800, y + outsideDirY * 800);
			generateZombieTime = 100 + rand() % 350;
		}
	}
}

void Window::draw(float scrollX, float scrollY) {
	Actor::draw(scrollX, scrollY);

	// los frames de la animación representan como de destruida esta la barrera
	// elige el frame según la vida de la barrera
	if (barrierHealth <= 0) {
		barrier->currentFrame = 3;
	} 
	else if (barrierHealth <= 1000) {
		barrier->currentFrame = 2;
	}
	else if (barrierHealth <= 2000) {
		barrier->currentFrame = 1;
	}
	else {
		barrier->currentFrame = 0;
	}
	barrier->source.x = barrier->currentFrame * barrier->frameWidth;
	barrier->draw(x - scrollX, y - scrollY);
}

bool Window::isActorInRebuildBarrierRange(Actor* actor) {
	return distanceTo(actor) < 96;
}

void Window::rebuildBarrier(int healthAmount) {
	if (barrierHealth < WINDOW_BARRIER_MAX_HEALTH) {
		barrierHealth = min(WINDOW_BARRIER_MAX_HEALTH, barrierHealth + healthAmount);
	}
}

void Window::damageBarrier(int healthAmount) {
	if (barrierHealth > 0) {
		barrierHealth = max(0, barrierHealth - healthAmount);
	}
}

bool Window::canReachPlayer() {
	return game->gameLayer->pathFinding->hasPathFrom(x + insideDirX * 200, y + insideDirY * 200);
}
