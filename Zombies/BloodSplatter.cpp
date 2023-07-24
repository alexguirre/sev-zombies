#include "BloodSplatter.h"

BloodSplatter::BloodSplatter(float x, float y, Game* game)
	: Actor(getRandomTextureName(), x, y, 128, 128, game) {
	rotation = rand() % 360;
}


const char* BloodSplatter::getRandomTextureName() {
	const char* textures[]{
		"res/level/blood_0.png",
		"res/level/blood_1.png",
		"res/level/blood_2.png",
		"res/level/blood_3.png",
		"res/level/blood_4.png",
		"res/level/blood_5.png",
		"res/level/blood_6.png",
	};

	return textures[rand() % size(textures)];
}
