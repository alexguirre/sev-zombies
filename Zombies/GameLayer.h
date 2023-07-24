#pragma once

#include "Layer.h"
#include "Player.h"

#include "Zombie.h"
#include "Projectile.h"
#include "Grenade.h"
#include "Text.h"
#include "Audio.h"
#include "Tile.h"
#include "Space.h"
#include "Window.h"
#include "Door.h"
#include "WeaponLocation.h"
#include "PathFinding.h"
#include "HealthBar.h"
#include "ScoreFloatingText.h"
#include "WeaponIcon.h"
#include "BloodSplatter.h"

#include <fstream> // Leer ficheros
#include <sstream> // Leer líneas / String
#include <list>
#include <vector>

class GameLayer : public Layer
{
public:
	GameLayer(Game* game);
	void init() override;
	void processControls() override;
	void update() override;
	void draw() override;
	void keysToControls(SDL_Event event);
	void mouseToControls(SDL_Event event); // USO DE MOUSE
	void loadMap(const string& name);
	void loadMapObject(char character, int mapX, int mapY, const vector<string>& map);
	void calculateScroll();
	void shakeCamera();
	void createProjectile(float x, float y, float vx, float vy, int damage, int hitScore, int timeToLive, bool isBullet, bool fromZombie);
	void createGrenade(float x, float y, float vx, float vy, int damage, int timeToLive);
	void createZombie(float x, float y);
	void createBloodSplatter(float x, float y);
	void addScore(int scorePoints, float x, float y);
	void showHelpText(const string& helpTextString);
	void drawDebugBoundingBox(Actor* actor, int r, int g, int b);

	Space* space;
	PathFinding* pathFinding;
	float scrollX;
	float scrollY;
	float cameraShakeScrollX;
	float cameraShakeScrollY;
	float cameraShakeRadius;
	int cameraShakeTime;
	int mapWidth;
	int mapHeight;
	list<Tile*> floorTiles;
	list<Tile*> wallTiles;
	list<Window*> windows;
	list<Door*> doors;
	list<WeaponLocation*> weaponLocations;
	Player* player;
	list<Projectile*> projectiles;
	list<Grenade*> grenades;
	list<Zombie*> zombies;
	list<BloodSplatter*> bloodSplatters;
	bool pause;
	bool dead;

	int scoreRecord;
	int score;
	int doorPrice;

	Audio* audioBackgroundMusic;
	Audio* audioDoorOpen;
	Audio* audioBuy;

	// HUD
	Actor* crosshair;
	Text* pauseText;
	Actor* deadScreenBackground;
	Text* deadText;
	Text* deadScoreText;
	Text* deadRecordText;
	Text* helpText;
	Text* scoreText;
	HealthBar* healthBar;
	HealthBar* rebuildBarrierBar;
	bool rebuildBarrierBarVisible;
	WeaponIcon* weapon1Icon;
	WeaponIcon* weapon2Icon;
	list<ScoreFloatingText*> scoreFloatingTexts;

	// Opciones de debug
	bool debugShowBoundingBoxes;
	bool debugShowPathFindingMap;

	// Controles
	bool controlPause = false;
	int controlMoveX = 0;
	int controlMoveY = 0;
	int controlAimX = 0;
	int controlAimY = 0;
	bool controlBuy = false;
	bool controlRebuildBarrier = false;
	bool controlSwitchWeapon = false;
	bool controlShoot = false;
	bool controlKnifeAttack = false;
	bool controlDeadRestart = false;
	bool controlDebugTogglePathfindingMap = false;
	bool controlDebugToggleBoundingBoxes = false;
	bool controlDebugAddScore = false;

};

