#include "HealthBar.h"

HealthBar::HealthBar(float x, float y, int maxHealth, Game* game) 
	: Actor("res/hud/health_background.png", x, y, 250, 25, game) {
	this->foregroundTexture = game->getTexture("res/hud/health_foreground.png");
	this->text = new Text("", x, y, game);
	this->maxHealth = maxHealth;
	this->currentHealth = maxHealth;
}

void HealthBar::draw(float scrollX, float scrollY) {
	Actor::draw(scrollX, scrollY); // dibujar el fondo

	float healthPercentage = (float)currentHealth / maxHealth;

	SDL_Rect source;
	source.x = 0;
	source.y = 0;
	source.w = fileWidth * healthPercentage;
	source.h = fileHeight;

	SDL_Rect destination;
	destination.x = x - scrollX - width / 2;
	destination.y = y - scrollY - height / 2;
	destination.w = width * healthPercentage;
	destination.h = height;

	SDL_RenderCopyEx(game->renderer, foregroundTexture, &source, &destination, 0, NULL, SDL_FLIP_NONE);

	text->x = x - scrollX;
	text->y = y - scrollY;
	text->content = to_string(currentHealth) + " / " + to_string(maxHealth);
	text->draw();
}
