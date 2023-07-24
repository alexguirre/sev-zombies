#pragma once

#include "Actor.h"

#define TILE_WIDTH 64
#define TILE_HEIGHT 64

struct TileTextureCoords { int x, y; };

enum TileNeighbours
{
	TN_NORTH = 1 << 0,
	TN_WEST = 1 << 1,
	TN_EAST = 1 << 2,
	TN_SOUTH = 1 << 3,
	TN_NORTHWEST = 1 << 4,
	TN_NORTHEAST = 1 << 5,
	TN_SOUTHWEST = 1 << 6,
	TN_SOUTHEAST = 1 << 7,

	TN_NUM_NEIGHBOURS = 8,
};

class Tile : public Actor
{
public:
	Tile(string filename, int neighboursBitset, float x, float y, Game* game);
	void draw(float scrollX = 0, float scrollY = 0) override;
	TileTextureCoords getTextureCoords();

	int neighboursBitset;
};
