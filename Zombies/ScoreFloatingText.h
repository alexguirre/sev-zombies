#pragma once
#include "Actor.h"
#include "Text.h"

class ScoreFloatingText : public Actor
{
public:
	ScoreFloatingText(float x, float y, int scorePoints, Game* game);
	virtual void draw(float scrollX = 0, float scrollY = 0);
	void update();

	Text* text;
	bool shouldBeDeleted;
	int timeToLive;
};

