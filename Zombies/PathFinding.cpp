#include "PathFinding.h"
#include "GameLayer.h"
#include <limits>
#include <queue>

PathFinding::PathFinding(const vector<string>& map, Game* game) {
	this->game = game;
	mapHeight = map.size();
	mapWidth = map[0].size();

	obstaclesMap.resize(mapWidth * mapHeight, true);
	costMap.resize(mapWidth * mapHeight, -1);

	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			obstaclesMap[getIndex(x, y)] = map[y][x] != '.' && map[y][x] != 'Z' && map[y][x] != 'P' && map[y][x] != '1' && map[y][x] != '2' && map[y][x] != '3';
		}
	}

	debugText = new Text("", 0, 0, game);
}

void PathFinding::updateTargetPosition(float targetX, float targetY) {
	debugX.clear();
	debugY.clear();

	fill(costMap.begin(), costMap.end(), -1); // inicialmente, el mapa de costes es todo -1 (sin camino hacia el objetivo)

	int targetMapX, targetMapY;
	getMapCoord(targetX, targetY, targetMapX, targetMapY);

	if (!isInsideMap(targetMapX, targetMapY) || obstaclesMap[getIndex(targetMapX, targetMapY)]) {
		return;
	}

	map<int, Zombie*> zombies; // generar un map de indice de matriz a Zombie* para saber en que celdas hay zombies
	for (Zombie* zombie : game->gameLayer->zombies) {
		int zombieMapX, zombieMapY;
		getMapCoord(zombie->x, zombie->y, zombieMapX, zombieMapY);
		if (isInsideMap(zombieMapX, zombieMapY)) {
			zombies[getIndex(zombieMapX, zombieMapY)] = zombie;
		}
	}

	// rellena el mapa de costes usando breadth-first search, empezando desde el objetivo
	struct Cell { int x, y, prevCost; };
	queue<Cell> visitQueue;
	visitQueue.push({ targetMapX, targetMapY, -1 });

	while (!visitQueue.empty()) {
		Cell cell = visitQueue.front();
		visitQueue.pop();

		int i = getIndex(cell.x, cell.y);
		int addedCost = 1;
		if (zombies.find(i) != zombies.end()) {
			// si hay un zombie en esta celda, añadir un coste extra para que los zombies intenten ir por caminos diferentes
			addedCost += 1;
		}
		costMap[i] = cell.prevCost + addedCost;

		for (int offsetY = -1; offsetY <= 1; offsetY++) {
			for (int offsetX = -1; offsetX <= 1; offsetX++) {
				if (offsetY != 0 || offsetX != 0) {
					int nextX = cell.x + offsetX;
					int nextY = cell.y + offsetY;
					int nextIdx = getIndex(nextX, nextY);
					if (isInsideMap(nextX, nextY) && costMap[nextIdx] == -1 && !obstaclesMap[nextIdx]) {
						costMap[nextIdx] = -2; // un numero diferente a -1 para indicar que ya se va a visitar esta celda
						visitQueue.push({ nextX, nextY, cell.prevCost + addedCost });
					}
				}
			}
		}
	}
}

void PathFinding::getMoveTarget(float x, float y, float& outTargetX, float& outTargetY) {
	outTargetX = 0.0f;
	outTargetY = 0.0f;

	int mapX, mapY;
	getMapCoord(x, y, mapX, mapY);

	if (!isInsideMap(mapX, mapY) || obstaclesMap[getIndex(mapX, mapY)]) {
		return;
	}

	Tile* tile = getFloorTileForWorldCoord(x, y);
	if (!tile) {
		return;
	}

	// obtener que offsets se pueden alcanzar desde la tile actual
	struct Offset { int x, y; };
	Offset initialOffsets[TN_NUM_NEIGHBOURS]{ {0, -1}, {-1, 0}, {1, 0}, {0, 1}, {-1, -1}, {1, -1}, {-1, 1}, {1, 1} };
	vector<Offset> offsets;
	for (int i = 0; i < TN_NUM_NEIGHBOURS; i++) {
		if (tile->neighboursBitset & (1 << i)) {
			offsets.push_back(initialOffsets[i]);
		}
	}

	// ordena los offsets, los más cercanos a la posición actual primero para intentar evitar cambios de dirección muy bruscos
	sort(offsets.begin(), offsets.end(),
		[&](auto& a, auto& b) { 
			float ax, ay, bx, by;
			getWorldCoord(mapX + a.x, mapY + a.y, ax, ay);
			getWorldCoord(mapX + b.x, mapY + b.y, bx, by);
			return Game::distanceBetween(x, y, ax, ay) < Game::distanceBetween(x, y, bx, by);
		});

	// buscar el offset con el menor coste
	int minCost = -1, matchX, matchY;
	int i = 0;
	for (auto offset : offsets) {
		int nextX = mapX + offset.x;
		int nextY = mapY + offset.y;
		if (isInsideMap(nextX, nextY)) {
			int cost = costMap[getIndex(nextX, nextY)];
			if (cost != -1) {
				if (minCost == -1 || cost < minCost) {
					minCost = cost;
					matchX = nextX;
					matchY = nextY;

					getWorldCoord(nextX, nextY, outTargetX, outTargetY);
					outTargetX += TILE_WIDTH / 2;
					outTargetY += TILE_HEIGHT / 2;
				}
			}
		}
		i++;
	}

	if (minCost != -1) {
		debugX.push_back(matchX);
		debugY.push_back(matchY);
	}
}

void PathFinding::debugDraw(float scrollX, float scrollY) {
	SDL_Rect cellBounds;
	cellBounds.w = TILE_WIDTH;
	cellBounds.h = TILE_HEIGHT;

	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			int cost = costMap[getIndex(x, y)];

			cellBounds.x = x * TILE_WIDTH - scrollX;
			cellBounds.y = y * TILE_HEIGHT - scrollY;
			if (cost == -1) {
				SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 200);
			}
			else {
				SDL_SetRenderDrawColor(game->renderer, 0, 255, 0, 200 - min(cost * 15, 200));
			}
			SDL_RenderFillRect(game->renderer, &cellBounds);

			debugText->x = cellBounds.x + TILE_WIDTH / 2;
			debugText->y = cellBounds.y + TILE_HEIGHT / 2;
			debugText->content = to_string(cost);
			debugText->draw();
		}
	}

	for (int i = 0; i < debugX.size(); i++) {
		cellBounds.x = debugX[i] * TILE_WIDTH - scrollX;
		cellBounds.y = debugY[i] * TILE_HEIGHT - scrollY;
		SDL_SetRenderDrawColor(game->renderer, 0, 0, 200, 200);
		SDL_RenderFillRect(game->renderer, &cellBounds);
	}
	debugX.clear();
	debugY.clear();
}

void PathFinding::getMapCoord(float x, float y, int& outMapX, int& outMapY) {
	outMapX = x / TILE_WIDTH;
	outMapY = y / TILE_HEIGHT;
}

void PathFinding::getWorldCoord(int mapX, int mapY, float& outX, float& outY) {
	outX = mapX * TILE_WIDTH;
	outY = mapY * TILE_HEIGHT;
}

Tile* PathFinding::getFloorTileForWorldCoord(float x, float y) {
	for (Tile* tile : game->gameLayer->floorTiles) {
		float left = tile->x - tile->width / 2;
		float right = tile->x + tile->width / 2;
		float top = tile->y - tile->height / 2;
		float bottom = tile->y + tile->height / 2;

		if (x >= left && x < right && y >= top && y < bottom) {
			return tile;
		}
	}

	return nullptr;
}

bool PathFinding::isInsideMap(int mapX, int mapY) {
	return mapX >= 0 && mapX < mapWidth && mapY >= 0 && mapY < mapHeight;
}

int PathFinding::getIndex(int mapX, int mapY) {
	return mapY * mapWidth + mapX;
}

void PathFinding::removeObstacle(float x, float y) {
	int mapX, mapY;
	getMapCoord(x, y, mapX, mapY);

	if (isInsideMap(mapX, mapY)) {
		obstaclesMap[getIndex(mapX, mapY)] = false;
	}
}

bool PathFinding::hasPathFrom(float x, float y) {
	int mapX, mapY;
	getMapCoord(x, y, mapX, mapY);

	int cost = -1;
	if (isInsideMap(mapX, mapY)) {
		cost = costMap[getIndex(mapX, mapY)];
	}

	return cost != -1;
}
