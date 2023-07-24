#pragma once

#include "Game.h"

class Text
{
public:
	Text(string content, float x, float y, Game* game);
	void draw();
	string content; // texto
	int x;
	int y;
	int width;
	int height;
	SDL_Color color;
	TTF_Font* font;
	Game* game; // referencia al juego
};

