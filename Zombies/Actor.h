#pragma once

#include "Game.h"
#include <list>

enum CollisionFlags
{
	COLLISION_WALL = 1 << 0,
	COLLISION_WINDOW = 1 << 1,
	COLLISION_ALL = ~0,
};

class Actor
{
public:
	Actor(string filename, float x, float y, int width, int height, Game* game);
	~Actor();
	virtual void draw(float scrollX = 0, float scrollY = 0);
	bool isOverlap(Actor* actor);
	bool isInRender(float scrollX = 0, float scrollY = 0);
	bool containsPoint(int pointX, int pointY); // contiene punto
	float distanceTo(float x, float y);
	float distanceTo(Actor* actor);

	SDL_Texture* texture;
	int x;
	int y;
	float rotation; // en grados
	float vx;
	float vy;
	bool bounce;
	float bounciness;
	int width;
	int height;
	int fileWidth;
	int fileHeight;
	int staticActorCollisionFlags;
	int dynamicActorCollidesWith;
	Game* game; // referencia al juego
};

