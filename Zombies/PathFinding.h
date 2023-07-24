#pragma once
#include "Game.h"
#include <vector>
#include <string>
#include "Text.h"
#include "Tile.h"

class PathFinding
{
public:
	PathFinding(const vector<string>& map, Game* game);
	void updateTargetPosition(float targetX, float targetY); // calcula los costes para alcanzar el objetivo
	void getMoveTarget(float x, float y, float& outTargetX, float& outTargetY); // devuelve la posición hacia la que hay que moverse para alcanzar al objetivo
	void getMapCoord(float x, float y, int& outMapX, int& outMapY);
	void getWorldCoord(int mapX, int mapY, float& outX, float& outY);
	Tile* getFloorTileForWorldCoord(float x, float y);
	bool isInsideMap(int mapX, int mapY);
	int getIndex(int mapX, int mapY);
	void removeObstacle(float x, float y);
	bool hasPathFrom(float x, float y);
	void debugDraw(float scrollX, float scrollY);
	
	int mapWidth;  // ancho y altura de las matrices
	int mapHeight;
	vector<bool> obstaclesMap; // matriz con los puntos del mapa que no se pueden atravesar
	vector<int> costMap;       // matriz con los costes para alcanzar al objetivo
	Game* game;

	Text* debugText;
	vector<int> debugX, debugY; // lista de posiciones devueltas por getMoveTarget, para mostrarlas en debugDraw()
};

