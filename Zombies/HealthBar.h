#pragma once
#include "Actor.h"
#include "Text.h"

class HealthBar : public Actor
{
public:
	HealthBar(float x, float y, int maxHealth, Game* game);
	virtual void draw(float scrollX = 0, float scrollY = 0);

	SDL_Texture* foregroundTexture;
	Text* text;
	int maxHealth;
	int currentHealth;
};

