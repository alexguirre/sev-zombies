#pragma once
#include "Actor.h"

class Door : public Actor
{
public:
	Door(bool horizontal, float x, float y, Game* game);
	void draw(float scrollX = 0, float scrollY = 0) override;
	void open();
	bool isActorInOpenRange(Actor* actor);

	bool isOpen;
};
