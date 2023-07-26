#pragma once

// Todas las librerias de SDL y otras utilidades
#include <iostream>
#include <string>
using namespace std;
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <map> 

// Valores generales
#define WIDTH 1280
#define HEIGHT 720

#include "Layer.h"
class Layer;
class GameLayer;

class Audio;

class Game
{
public:
	Game();
	void scale();
	void loop();
	void update();
	SDL_Texture* getTexture(string filename);
	Audio* getAudio(string filename, bool loop);

	map<string, SDL_Texture*> mapTextures; // map - cache
	map<string, Audio*> mapAudios;
	TTF_Font* font;
	TTF_Font* fontSmall;
	TTF_Font* fontBig;
	bool scaledToMax = false;
	float scaleLower = 1;
	SDL_Window* window; // ventana
	SDL_Renderer* renderer; // renderizador
	bool loopActive; // Juego activo
	Layer* layer;
	Layer* menuLayer;
	GameLayer* gameLayer;

	int currentLevel = 0;
	int finalLevel = 3;


	static float distanceBetween(float fromX, float fromY, float toX, float toY);
	static float getRotationFromDirection(float dirX, float dirY);
	static void getDirectionFromRotation(float rotation, float& outDirX, float& outDirY);
};
