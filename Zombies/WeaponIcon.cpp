#include "WeaponIcon.h"

WeaponIcon::WeaponIcon(float x, float y, Weapon* weapon, Game* game)
	: Actor("res/empty.png", x, y, 1, 1, game) {
	this->ammoText = new Text("", x + 64, y, game);
}

void WeaponIcon::draw(float scrollX, float scrollY) {
	if (weapon == nullptr) {
		return;
	}

	Animation* icon = weapon->aHudIcon;
	icon->currentFrame = weapon->selected ? 0 : 1;
	icon->source.x = icon->currentFrame * icon->frameWidth;
	icon->draw(x, y);

	ammoText->content = to_string(weapon->ammo);
	ammoText->color = weapon->selected ? SDL_Color{ 255, 255, 255, 255 } : SDL_Color{ 143, 143, 143, 255 };
	ammoText->draw();
}
