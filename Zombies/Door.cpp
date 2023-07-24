#include "Door.h"
#include "GameLayer.h"

Door::Door(bool horizontal, float x, float y, Game* game)
	: Actor(horizontal ? "res/level/door_horizontal.png" : "res/level/door_vertical.png", x, y, horizontal ? 128 : 64, horizontal ? 64 : 128, game) {
	isOpen = false;

	staticActorCollisionFlags = COLLISION_WALL;
}

void Door::draw(float scrollX, float scrollY) {
	if (!isOpen) {
		Actor::draw(scrollX, scrollY);
	}
}

void Door::open() {
	isOpen = true;
	staticActorCollisionFlags = 0; // quitar la colisión de la puerta

	// hacer que la puerta ya no bloquee el camino al calcular el pathfinding
	if (width > height) { // horizontal
		game->gameLayer->pathFinding->removeObstacle(x - 32, y); // la puerta ocupa dos tiles
		game->gameLayer->pathFinding->removeObstacle(x + 32, y);
	}
	else { // vertical
		game->gameLayer->pathFinding->removeObstacle(x, y - 32);
		game->gameLayer->pathFinding->removeObstacle(x, y + 32);
	}

	game->gameLayer->audioDoorOpen->play();
}

bool Door::isActorInOpenRange(Actor* actor) {
	return distanceTo(actor) < 80;
}
