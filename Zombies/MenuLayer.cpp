#include "MenuLayer.h"
#include "GameLayer.h"

MenuLayer::MenuLayer(Game* game)
	: Layer(game) {
	init();
}

void MenuLayer::init() {
	background = new Actor("res/hud/menu_background.png", WIDTH * 0.5f, HEIGHT * 0.5f, 1280, 720, game);
	button = new Actor("res/hud/button_play.png", WIDTH * 0.225f, HEIGHT * 0.325f, 232, 72, game);
}

void MenuLayer::draw() {
	background->draw();
	button->draw();

	SDL_RenderPresent(game->renderer); // Renderiza NO PUEDE FALTAR
}

void MenuLayer::processControls() {
	// obtener controles
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		keysToControls(event);
		mouseToControls(event);
	}

	//procesar controles, solo tiene uno
	if (controlStart) {
		// Cambia la capa
		SDL_ShowCursor(0);
		game->layer = game->gameLayer;
		controlStart = false;
	}
}

void MenuLayer::keysToControls(SDL_Event event) {
	if (event.type == SDL_KEYDOWN) {
		int code = event.key.keysym.sym;
		// Pulsada
		switch (code) {
		case SDLK_ESCAPE:
			game->loopActive = false;
			break;
		case SDLK_1:
			game->scale();
			break;
		case SDLK_SPACE:
			controlStart = true;
			break;
		}
	}
}

void MenuLayer::mouseToControls(SDL_Event event) {
	// Modificación de coordenadas por posible escalado
	float motionX = event.motion.x / game->scaleLower;
	float motionY = event.motion.y / game->scaleLower;

	// Cada vez que hacen click
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (button->containsPoint(motionX, motionY)) {
			controlStart = true;
		}
	}
}
