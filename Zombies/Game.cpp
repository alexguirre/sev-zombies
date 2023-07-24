#include "Game.h"
#include "GameLayer.h"
#include "MenuLayer.h"
#include "Audio.h"

Game::Game() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "Error SDL_Init" << SDL_GetError() << endl;
	}
	if (SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer) < 0) {
		cout << "Error Window y Renderer" << SDL_GetError() << endl;
	}
	SDL_SetWindowTitle(window, "Zombies");
	// Escalado de imágenes de calidad 
	// https://wiki.libsdl.org/SDL_HINT_RENDER_SCALE_QUALITY
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	// fuentes
	TTF_Init();
	font = TTF_OpenFont("res/hud/sans.ttf", 24);
	fontSmall = TTF_OpenFont("res/hud/sans.ttf", 18);
	fontBig = TTF_OpenFont("res/hud/sans.ttf", 50);
	TTF_SetFontStyle(fontBig, TTF_STYLE_BOLD);

	menuLayer = new MenuLayer(this);
	gameLayer = new GameLayer(this);
	layer = menuLayer;

	loopActive = true; // bucle activo
	loop();
}


void Game::loop() {
	int initTick; // ms de inicio loop
	int endTick; // ms de fin de loop
	int differenceTick; // fin - inicio
	while (loopActive) {
		initTick = SDL_GetTicks();

		// Controles
		layer->processControls();
		// Actualizar elementos
		layer->update();
		// Dibujar
		layer->draw();


		endTick = SDL_GetTicks();
		differenceTick = endTick - initTick;

		if (differenceTick < (1000 / 30)) {
			SDL_Delay((1000 / 30) - differenceTick);
		}
	}
}

void Game::scale() {
	scaledToMax = !scaledToMax;

	if (scaledToMax) {
		SDL_DisplayMode PCdisplay;
		SDL_GetCurrentDisplayMode(0, &PCdisplay);
		float scaleX = (float)PCdisplay.w / (float)WIDTH;
		float scaleY = (float)PCdisplay.h / (float)HEIGHT;
		// Necesitamos la menor de las 2 escalas para no deformar el juego
		scaleLower = scaleX;
		if (scaleY < scaleX) {
			scaleLower = scaleY;
		}
		// Cambiar dimensiones ventana
		SDL_SetWindowSize(window, WIDTH * scaleLower, HEIGHT * scaleLower);
		// Cambiar escala del render
		SDL_RenderSetScale(renderer, scaleLower, scaleLower);
	}
	else { // Escala Original
		scaleLower = 1;
		// Cambiar dimensiones ventana
		SDL_SetWindowSize(window, WIDTH, HEIGHT);
		// Cambiar escala del render
		SDL_RenderSetScale(renderer, 1, 1);
	}

}

SDL_Texture* Game::getTexture(string filename) {
	if (mapTextures.find(filename) == mapTextures.end()) {
		cout << "Nueva textura " << filename << endl;
		SDL_Surface* surface = IMG_Load(filename.c_str());
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		mapTextures[filename] = texture;
	}

	return mapTextures[filename];
}

Audio* Game::getAudio(string filename, bool loop) {
	if (mapAudios.find(filename) == mapAudios.end()) {
		cout << "Nuevo audio " << filename << endl;
		mapAudios[filename] = new Audio(filename, loop);
	}

	return mapAudios[filename];
}

float Game::distanceBetween(float fromX, float fromY, float toX, float toY) {
	float diffX = toX - fromX;
	float diffY = toY - fromY;
	return sqrtf(diffX * diffX + diffY * diffY);
}

float Game::getRotationFromDirection(float dirX, float dirY) {
	float rotationRadians = atan2f(dirY, dirX);
	return rotationRadians * (360.0f / (2.0f * M_PI)); // convertir a grados
}

void Game::getDirectionFromRotation(float rotation, float& outDirX, float& outDirY) {
	float rotationRadians = rotation * ((2.0f * M_PI) / 360.0f); // convertir a radianes
	outDirX = cosf(rotationRadians);
	outDirY = sinf(rotationRadians);
}
