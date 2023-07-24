#include "Player.h"
#include "GameLayer.h"
#include "Shotgun.h"
#include "Pistol.h"
#include "GrenadeLauncher.h"

Player::Player(float x, float y, Game* game)
	: Actor("res/empty.png", x, y, 60, 60, game) {
	weapon1 = new Pistol(game);
	weapon2 = nullptr;
	weapon1->ammo = 20;
	currentWeapon = weapon1;
	currentWeapon->selected = true;

	const float SCALE_FACTOR = 0.425f;
	aKnifeAttack = new Animation("res/player/knife_attack.png", 375 * SCALE_FACTOR, 330 * SCALE_FACTOR, 5625, 330, 0, 15, false, game);
	animation = currentWeapon->aPlayerIdle;

	aFeetIdle = new Animation("res/player/feet_idle.png", 132 * SCALE_FACTOR, 155 * SCALE_FACTOR, 132, 155, 1, 1, true, game);
	aFeetMove = new Animation("res/player/feet_move.png", 204 * SCALE_FACTOR, 124 * SCALE_FACTOR, 4080, 124, 1, 20, true, game);
	aFeetMoveLeft = new Animation("res/player/feet_move_left.png", 155 * SCALE_FACTOR, 174 * SCALE_FACTOR, 3100, 174, 0, 20, true, game);
	aFeetMoveRight = new Animation("res/player/feet_move_right.png", 154 * SCALE_FACTOR, 176 * SCALE_FACTOR, 3080, 176, 0, 20, true, game);
	animationFeet = aFeetIdle;

	audioWeaponSwitch = game->getAudio("res/audio/weapon_switch.wav", false);
	audioDamage[0] = game->getAudio("res/audio/player_damage_0.wav", false);
	audioDamage[1] = game->getAudio("res/audio/player_damage_1.wav", false);
	audioDamage[2] = game->getAudio("res/audio/player_damage_2.wav", false);
	audioDamage[3] = game->getAudio("res/audio/player_damage_3.wav", false);

	rotation = 0;
	health = PLAYER_MAX_HEALTH;
	damageRedFlashTime = 0;
	damagePlayAudioDelay = 0;
}

void Player::update() {
	animationFeet->update();
	if (vx == 0 && vy == 0) {
		animationFeet = aFeetIdle;
	}
	else {
		float velocityRotation = Game::getRotationFromDirection(vx, vy);

		// diferencia en grados entre la dirección en que se está moviendo y la de apuntado
		float diff = fmodf((velocityRotation - rotation) + 360.0f, 360.0f);

		if (diff >= 45.0f && diff <= 135.0f) {
			animationFeet = aFeetMoveRight;
		}
		else if (diff >= 225.0f && diff <= 315.0f) {
			animationFeet = aFeetMoveLeft;
		}
		else {
			animationFeet = aFeetMove;
		}
	}

	bool endAnimation = animation->update();

	if (animation->loop || endAnimation) {
		if (vx == 0 && vy == 0) {
			animation = currentWeapon->aPlayerIdle;
		}
		else {
			animation = currentWeapon->aPlayerMove;
		}
	}

	if (animation == aKnifeAttack) {
		// crea los proyectiles del ataque con cuchillo
		if (animation->updateTime == 0) { // el frame de la animación acaba de cambiar
			const int KNIFE_DAMAGE = 200, KNIFE_HIT_SCORE = 50;
			if (animation->currentFrame == 6) {
				float dirX, dirY;
				Game::getDirectionFromRotation(rotation - 30.0f, dirX, dirY);
				game->gameLayer->createProjectile(x + dirX * 65, y + dirY * 65, 0, 0, KNIFE_DAMAGE, KNIFE_HIT_SCORE, 5, false, false);
			}
			else if (animation->currentFrame == 8) {
				float dirX, dirY;
				Game::getDirectionFromRotation(rotation, dirX, dirY);
				game->gameLayer->createProjectile(x + dirX * 65, y + dirY * 65, 0, 0, KNIFE_DAMAGE, KNIFE_HIT_SCORE, 5, false, false);
			}
			else if (animation->currentFrame == 10) {
				float dirX, dirY;
				Game::getDirectionFromRotation(rotation + 30.0f, dirX, dirY);
				game->gameLayer->createProjectile(x + dirX * 65, y + dirY * 65, 0, 0, KNIFE_DAMAGE, KNIFE_HIT_SCORE, 5, false, false);
			}
		}
	}

	if (damageRedFlashTime > 0) {
		damageRedFlashTime--;
	}

	if (damagePlayAudioDelay > 0) {
		damagePlayAudioDelay--;
	}

	currentWeapon->update();
}

void Player::draw(float scrollX, float scrollY) {
	if (health <= 0) {
		return; // no dibujar al jugador si murió
	}

	bool redFlash = damageRedFlashTime > 0;
	if (redFlash) {
		SDL_SetTextureColorMod(animationFeet->texture, 200, 0, 0);
		SDL_SetTextureColorMod(animation->texture, 200, 0, 0);
	}
	animationFeet->draw(x - scrollX, y - scrollY, rotation);
	animation->draw(x - scrollX, y - scrollY, rotation);
	if (redFlash) {
		SDL_SetTextureColorMod(animationFeet->texture, 255, 255, 255);
		SDL_SetTextureColorMod(animation->texture, 255, 255, 255);
	}
}

void Player::moveX(float axis) {
	vx = axis * 18;
}

void Player::moveY(float axis) {
	vy = axis * 18;
}

void Player::aimAt(float x, float y) {
	// Dirección desde el jugador hasta donde está apuntando
	float dirX = x - this->x;
	float dirY = y - this->y;
	rotation = Game::getRotationFromDirection(dirX, dirY);
}

void Player::knifeAttack() {
	if (animation != aKnifeAttack) {
		aKnifeAttack->currentFrame = 0;
		animation = aKnifeAttack;
	}
}

void Player::shoot() {
	if (currentWeapon->shoot(x, y, rotation)) {
		currentWeapon->aPlayerShoot->currentFrame = 0;
		animation = currentWeapon->aPlayerShoot;
	}
}

void Player::receiveDamage(int damage) {
	if (health > 0) {
		health = max(0, health - damage);
	}
	if (damagePlayAudioDelay == 0) {
		audioDamage[rand() % size(audioDamage)]->play();
		damagePlayAudioDelay = 30;
	}
	damageRedFlashTime = 4;
}

void Player::switchWeapon() {
	if (weapon2 == nullptr) {
		// si solo tiene un arma, no puede cambiarla
		return;
	}

	audioWeaponSwitch->play();
	currentWeapon->selected = false;
	currentWeapon = currentWeapon == weapon1 ? weapon2 : weapon1;
	currentWeapon->selected = true;
}
