#include "ScoreFloatingText.h"

ScoreFloatingText::ScoreFloatingText(float x, float y, int scorePoints, Game* game)
	: Actor("res/empty.png", x, y, 1, 1, game) {
	this->text = new Text((scorePoints >= 0 ? "+" : "") + to_string(scorePoints), x, y, game);
	this->text->font = game->fontSmall;

	shouldBeDeleted = false;
	timeToLive = 16;
}

void ScoreFloatingText::draw(float scrollX, float scrollY) {
	text->x = x - scrollX;
	text->y = y - scrollY;
	if (timeToLive <= 8) {
		text->color.a = 255 * (timeToLive / 8.0f);
	}
	text->draw();
}

void ScoreFloatingText::update() {
	y -= 4;
	timeToLive--;
	shouldBeDeleted = timeToLive <= 0;
}
