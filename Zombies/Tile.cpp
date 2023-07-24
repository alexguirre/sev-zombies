	#include "Tile.h"

#include "Text.h"

Tile::Tile(string filename, int neighboursBitset, float x, float y, Game* game)
	: Actor(filename, x, y, TILE_WIDTH, TILE_HEIGHT, game) {
	this->neighboursBitset = neighboursBitset;

	staticActorCollisionFlags = COLLISION_WALL;
}

void Tile::draw(float scrollX, float scrollY) {
	TileTextureCoords textureCoords = getTextureCoords();

	// Recorte en el fichero de la imagen
	SDL_Rect source;
	source.x = textureCoords.x * TILE_WIDTH;
	source.y = textureCoords.y * TILE_HEIGHT;
	source.w = TILE_WIDTH;
	source.h = TILE_HEIGHT;

	// Donde se va a pegar en el renderizador
	SDL_Rect destination;
	destination.x = x - width / 2 - scrollX;
	destination.y = y - height / 2 - scrollY;
	destination.w = width;
	destination.h = height;
	// Modificar para que la referencia sea el punto central

	SDL_RenderCopyEx(game->renderer,
		texture, &source, &destination, 0, NULL, SDL_FLIP_NONE);
}

TileTextureCoords Tile::getTextureCoords() {
	switch (neighboursBitset)
	{
	case TN_WEST | TN_EAST | TN_SOUTH:  return { 3, 0 };
	case TN_WEST | TN_EAST | TN_NORTH:  return { 4, 0 };
	case TN_NORTH | TN_SOUTH | TN_EAST: return { 3, 1 };
	case TN_NORTH | TN_SOUTH | TN_WEST: return { 4, 1 };

	case TN_SOUTH | TN_EAST: return { 0, 0 };
	case TN_SOUTH | TN_WEST: return { 1, 0 };
	case TN_NORTH | TN_EAST: return { 0, 1 };
	case TN_NORTH | TN_WEST: return { 1, 1 };

	case TN_NORTH: return { 6, 0 };
	case TN_SOUTH: return { 5, 1 };
	case TN_WEST:  return { 5, 0 };
	case TN_EAST:  return { 6, 1 };

	case TN_WEST | TN_EAST:   return { 2, 1 };
	case TN_NORTH | TN_SOUTH: return { 2, 0 };

	case TN_WEST | TN_EAST | TN_NORTH | TN_SOUTH: return { 7, 0 };

	case TN_EAST | TN_SOUTH | TN_SOUTHEAST: return { 7, 1 };
	case TN_EAST | TN_NORTH | TN_NORTHEAST: return { 7, 2 };
	case TN_WEST | TN_SOUTH | TN_SOUTHWEST: return { 7, 3 };
	case TN_WEST | TN_NORTH | TN_NORTHWEST: return { 7, 4 };

	case TN_NORTH | TN_SOUTH | TN_EAST | TN_NORTHEAST | TN_SOUTHEAST: return { 4, 2 };
	case TN_NORTH | TN_SOUTH | TN_WEST | TN_NORTHWEST | TN_SOUTHWEST: return { 5, 2 };
	case TN_WEST | TN_EAST | TN_SOUTH | TN_SOUTHWEST | TN_SOUTHEAST:  return { 4, 3 };
	case TN_WEST | TN_EAST | TN_NORTH | TN_NORTHWEST | TN_NORTHEAST:  return { 5, 3 };

	case TN_NORTH | TN_SOUTH | TN_WEST | TN_EAST | TN_NORTHWEST | TN_NORTHEAST | TN_SOUTHWEST: return { 0, 2 };
	case TN_NORTH | TN_SOUTH | TN_WEST | TN_EAST | TN_NORTHWEST | TN_NORTHEAST | TN_SOUTHEAST: return { 1, 2 };
	case TN_NORTH | TN_SOUTH | TN_WEST | TN_EAST | TN_NORTHWEST | TN_SOUTHWEST | TN_SOUTHEAST: return { 0, 3 };
	case TN_NORTH | TN_SOUTH | TN_WEST | TN_EAST | TN_NORTHEAST | TN_SOUTHWEST | TN_SOUTHEAST: return { 1, 3 };

	case TN_NORTH | TN_SOUTH | TN_EAST | TN_SOUTHEAST: return { 2, 4 };
	case TN_NORTH | TN_SOUTH | TN_WEST | TN_SOUTHWEST: return { 3, 4 };
	case TN_NORTH | TN_SOUTH | TN_EAST | TN_NORTHEAST: return { 2, 5 };
	case TN_NORTH | TN_SOUTH | TN_WEST | TN_NORTHWEST: return { 3, 5 };

	case TN_WEST | TN_EAST | TN_SOUTH | TN_SOUTHEAST: return { 4, 4 };
	case TN_WEST | TN_EAST | TN_SOUTH | TN_SOUTHWEST: return { 5, 4 };
	case TN_WEST | TN_EAST | TN_NORTH | TN_NORTHEAST: return { 4, 5 };
	case TN_WEST | TN_EAST | TN_NORTH | TN_NORTHWEST: return { 5, 5 };

	case TN_NORTH | TN_SOUTH | TN_WEST | TN_EAST | TN_NORTHEAST | TN_SOUTHWEST: return { 6, 4 };
	case TN_NORTH | TN_SOUTH | TN_WEST | TN_EAST | TN_NORTHWEST | TN_SOUTHEAST: return { 6, 5 };

	case 0: return { 6, 3 };

	default: return { 7, 5 };
	}
}
