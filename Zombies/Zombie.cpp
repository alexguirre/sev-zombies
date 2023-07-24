#include "Zombie.h"
#include "GameLayer.h"
#include "Window.h"
#include <algorithm>

Zombie::Zombie(float x, float y, Game* game)
	: Actor("res/empty.png", x, y, 62, 62, game) {
	const float SCALE_FACTOR = 0.425f;
	aIdle = new Animation("res/zombie/idle.png", 241 * SCALE_FACTOR, 222 * SCALE_FACTOR, 4097, 222, 2, 17, true, game);
	aMove = new Animation("res/zombie/move.png", 228 * SCALE_FACTOR, 311 * SCALE_FACTOR, 4896, 311, 1, 17, true, game);
	aAttack = new Animation("res/zombie/attack.png", 318 * SCALE_FACTOR, 294 * SCALE_FACTOR, 2862, 294, 1, 9, false, game);
	animation = aIdle;

	vx = 0;
	vy = 0;
	rotation = 0;
	health = 500;
	damageRedFlashTime = 0;

	dynamicActorCollidesWith = 0; // no colisiona con nada para evitar que se quede atascado con los muros al moverse, el pathfinding evita que atraviese los muros
}

void Zombie::update() {
	bool endAnimation = animation->update();

	if (animation->loop || endAnimation) {
		if (vx == 0 && vy == 0) {
			animation = aIdle;
		} else {
			animation = aMove;
		}
	}

	if (animation == aAttack) {
		if (animation->updateTime == 0) { // el frame de la animación acaba de cambiar
			if (animation->currentFrame == 5) {
				float dirX, dirY;
				Game::getDirectionFromRotation(rotation, dirX, dirY);
				game->gameLayer->createProjectile(x + dirX * 62, y + dirY * 62, 0, 0, 50, 0, 4, false, true);
			}
		}
	}

	bool outside = (x < 0 || x > game->gameLayer->mapWidth) || (y < 0 || y > game->gameLayer->mapHeight);
	if (outside) {
		// el zombie está fuera del mapa, buscar una ventana por la que entrar
		if (targetWindow == nullptr) {
			// encontrar la ventana más cercana
			const auto& windows = game->gameLayer->windows;
			Window* closestWindow = nullptr;
			for (auto window : game->gameLayer->windows) {
				if (window->canReachPlayer() && (closestWindow == nullptr || distanceTo(window) < distanceTo(closestWindow))) {
					closestWindow = window;
				}
			}

			targetWindow = closestWindow;
		}
	}

	if (animation == aAttack || game->gameLayer->player->health <= 0) { // si está la animación de atacar o el jugador esta muerto parar de mover al zombie
		vxIntelligence = 0.0f;
		vyIntelligence = 0.0f;
	}
	else { // perseguir y atacar al jugador si esta vivo
		Actor* target = targetWindow ? (Actor*)targetWindow : game->gameLayer->player;
		if (distanceTo(target) < (min(target->width, target->height) + width * 0.425f)) {
			// está cerca del objetivo
			if (targetWindow && targetWindow->barrierHealth <= 0) {
				// el objetivo es la ventana y la barrera esta destruida, entrar al interior
				float targetInsideX = targetWindow->x + targetWindow->insideDirX * 200;
				float targetInsideY = targetWindow->y + targetWindow->insideDirY * 200;

				moveTo(targetInsideX, targetInsideY);

				if (!outside && Game::distanceBetween(x, y, targetInsideX, targetInsideY) < 128) {
					targetWindow = nullptr; // está dentro, quitar la ventana como objetivo
				}
			}
			else {
				// atacar al objetivo
				attack();
				rotation = Game::getRotationFromDirection(target->x - x, target->y - y);
			}
		}
		else {
			// mover el zombie hacia el objetivo
			float targetX, targetY;
			if (target == game->gameLayer->player) {
				game->gameLayer->pathFinding->getMoveTarget(x, y, targetX, targetY);
			}
			else {
				targetX = target->x;
				targetY = target->y;
			}

			moveTo(targetX, targetY);
		}
	}

	vx = vxIntelligence;
	vy = vyIntelligence;

	if (vx != 0.0f || vy != 0.0f) {
		// rotar en la dirección en la que se esta moviendo el zombie
		rotation = Game::getRotationFromDirection(vx, vy);
	}

	if (damageRedFlashTime > 0) {
		damageRedFlashTime--;
	}
}

void Zombie::moveTo(float targetX, float targetY) {
	const float MOVE_SPEED = 4.0f;

	float dirX = targetX - x;
	float dirY = targetY - y;
	if (dirX != 0.0f || dirY != 0.0f) {
		float dirLength = sqrtf(dirX * dirX + dirY * dirY);
		dirX /= dirLength;
		dirY /= dirLength;
		vxIntelligence = dirX * MOVE_SPEED;
		vyIntelligence = dirY * MOVE_SPEED;
	}
	else {
		vxIntelligence = 0.0f;
		vyIntelligence = 0.0f;
	}

	debugTargetXIntelligence = targetX;
	debugTargetYIntelligence = targetY;
}

void Zombie::draw(float scrollX, float scrollY) {
	bool redFlash = damageRedFlashTime > 0;
	if (redFlash) {
		SDL_SetTextureColorMod(animation->texture, 200, 0, 0);
	}
	animation->draw(x - scrollX, y - scrollY, rotation);
	if (redFlash) {
		SDL_SetTextureColorMod(animation->texture, 255, 255, 255);
	}


	if (game->gameLayer->debugShowPathFindingMap) {
		// dibujar el objectivo actual del zombie
		SDL_SetRenderDrawColor(game->renderer, 255, 0, 0, 255);
		SDL_RenderDrawLine(game->renderer, x - scrollX, y - scrollY, x - scrollX + vxIntelligence * 10.0f, y - scrollY + vyIntelligence * 10.0f);
	

		SDL_Rect targetBox;
		targetBox.x = debugTargetXIntelligence - scrollX - 8;
		targetBox.y = debugTargetYIntelligence - scrollY - 8;
		targetBox.w = 16;
		targetBox.h = 16;
		SDL_RenderFillRect(game->renderer, &targetBox);
	}
}

void Zombie::receiveDamage(int damage) {
	if (health > 0) {
		health = max(0, health - damage);
		if (health == 0) {
			game->gameLayer->addScore(100, x, y - 25);
			game->gameLayer->createBloodSplatter(x, y);
		}
	}
	damageRedFlashTime = 4;
}

void Zombie::attack() {
	if (animation != aAttack) {
		aAttack->currentFrame = 0;
		animation = aAttack;

		vxIntelligence = 0.0f;
		vyIntelligence = 0.0f;
	}
}
