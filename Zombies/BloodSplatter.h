#pragma once
#include "Actor.h"

class BloodSplatter : public Actor
{
public:
	BloodSplatter(float x, float y, Game* game);

	static const char* getRandomTextureName();
};

