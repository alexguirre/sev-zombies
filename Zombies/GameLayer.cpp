#include "GameLayer.h"

GameLayer::GameLayer(Game* game)
	: Layer(game) {
	scoreRecord = 0;
	pause = false;

	init();
}

void GameLayer::init() {
	audioBackgroundMusic = game->getAudio("res/audio/background_music.wav", true);
	audioBackgroundMusic->play();
	audioDoorOpen = game->getAudio("res/audio/door_open.wav", false);
	audioBuy = game->getAudio("res/audio/buy.wav", false);

	crosshair = new Actor("res/hud/crosshair.png", 0, 0, 64, 64, game);
	helpText = new Text("", WIDTH * 0.5f, HEIGHT * 0.85f, game);
	scoreText = new Text("", WIDTH * 0.9f, HEIGHT * 0.05f, game);
	healthBar = new HealthBar(WIDTH * 0.125f, HEIGHT * 0.05f, PLAYER_MAX_HEALTH, game);
	rebuildBarrierBar = new HealthBar(0, 0, WINDOW_BARRIER_MAX_HEALTH, game);
	rebuildBarrierBarVisible = false;
	weapon1Icon = new WeaponIcon(WIDTH * 0.055f, HEIGHT * 0.1f, nullptr, game);
	weapon2Icon = new WeaponIcon(WIDTH * 0.155f, HEIGHT * 0.1f, nullptr, game);
	scoreFloatingTexts.clear();
	pauseText = new Text("PAUSA", WIDTH * 0.5f, HEIGHT * 0.5f, game);
	pauseText->font = game->fontBig;
	deadScreenBackground = new Actor("res/hud/death_screen_background.png", WIDTH * 0.5f, HEIGHT * 0.5f, 428, 600, game);
	deadText = new Text("HAS MUERTO", WIDTH * 0.5f, HEIGHT * 0.325f, game);
	deadText->font = game->fontBig;
	deadText->color = { 180, 0, 0, 255 };
	deadScoreText = new Text("PUNTUACIÓN: 0", WIDTH * 0.5f, HEIGHT * 0.4f, game);
	deadRecordText = new Text("RECORD: 0", WIDTH * 0.5f, HEIGHT * 0.48f, game);

	score = 0;
	doorPrice = 200;

	space = new Space();

	scrollX = 0;
	scrollY = 0;
	cameraShakeScrollX = 0;
	cameraShakeScrollY = 0;
	cameraShakeRadius = 0;
	cameraShakeTime = 0;

	floorTiles.clear();
	wallTiles.clear();
	windows.clear();
	doors.clear();
	weaponLocations.clear();
	zombies.clear();
	projectiles.clear();
	grenades.clear();
	bloodSplatters.clear();

	pause = false;
	dead = false;

	loadMap("res/" + to_string(game->currentLevel) + ".txt");
}

void GameLayer::processControls() {
	// obtener controles
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		keysToControls(event);
		mouseToControls(event);
	}

	//procesar controles

	// controles de debug
	if (controlDebugTogglePathfindingMap) {
		debugShowPathFindingMap = !debugShowPathFindingMap;
		controlDebugTogglePathfindingMap = false;
	}
	if (controlDebugToggleBoundingBoxes) {
		debugShowBoundingBoxes = !debugShowBoundingBoxes;
		controlDebugToggleBoundingBoxes = false;
	}
	if (controlDebugAddScore) {
		score += 10000;
		controlDebugAddScore = false;
	}

	if (dead) {
		if (controlDeadRestart) {
			init();
			controlDeadRestart = false;
		}
		return; // no procesar más controles de juego si el jugador murió
	}

	if (controlPause) {
		pause = !pause;
		controlPause = false;
	}

	if (pause) { // no procesar controles de juego si esta pausado
		return;
	}

	// Eje X
	if (controlMoveX > 0) {
		player->moveX(1);
	}
	else if (controlMoveX < 0) {
		player->moveX(-1);
	}
	else {
		player->moveX(0);
	}

	// Eje Y
	if (controlMoveY > 0) {
		player->moveY(1);
	}
	else if (controlMoveY < 0) {
		player->moveY(-1);
	}
	else {
		player->moveY(0);
	}

	// Apuntado
	crosshair->x = controlAimX;
	crosshair->y = controlAimY;
	player->aimAt(controlAimX + scrollX, controlAimY + scrollY);

	if (controlSwitchWeapon) {
		player->switchWeapon();
		controlSwitchWeapon = false;
	}

	if (controlKnifeAttack) {
		player->knifeAttack();
		controlKnifeAttack = false;
	}

	if (controlShoot) {
		player->shoot();
	}

	rebuildBarrierBarVisible = false;
	if (controlRebuildBarrier) {
		// reparar la barrera si la hay dentro del rango
		for (auto window : windows) {
			if (window->barrierHealth < WINDOW_BARRIER_MAX_HEALTH && window->isActorInRebuildBarrierRange(player)) {
				window->rebuildBarrier(25);
				rebuildBarrierBarVisible = true;
				rebuildBarrierBar->currentHealth = window->barrierHealth;
				rebuildBarrierBar->x = window->x;
				rebuildBarrierBar->y = window->y;

				if (window->barrierHealth == WINDOW_BARRIER_MAX_HEALTH) {
					addScore(50, window->x, window->y);
				}
				break;
			}
		}
	}

	if (controlBuy) {
		bool doorOpened = false;
		if (score >= doorPrice) {
			// intentar abrir una puerta
			for (auto door : doors) {
				if (!door->isOpen && door->isActorInOpenRange(player)) {
					door->open();
					addScore(-doorPrice, door->x, door->y);
					doorPrice += 150;
					doorOpened = true;
					break;
				}
			}
		}

		if (!doorOpened) {
			// si no abrió ninguna puerta, intentar comprar un arma
			for (auto weaponLocation : weaponLocations) {
				if (score >= weaponLocation->price && weaponLocation->isActorInBuyRange(player)) {
					weaponLocation->buy(player);
					addScore(-weaponLocation->price, weaponLocation->x, weaponLocation->y);
					break;
				}
			}
		}

		controlBuy = false;
	}
}

void GameLayer::update() {
	if (pause) {
		return;
	}

	helpText->content.clear();

	if (cameraShakeTime > 0) {
		cameraShakeTime--;
	}

	space->update();

	list<Projectile*> deleteProjectiles;
	list<Grenade*> deleteGrenades;
	list<Zombie*> deleteZombies;

	for (auto projectile : projectiles) {
		projectile->update();

		if (projectile->shouldBeDeleted)
			deleteProjectiles.push_back(projectile);
	}

	for (auto grenade : grenades) {
		grenade->update();

		if (grenade->shouldBeDeleted)
			deleteGrenades.push_back(grenade);
	}

	if (player->health > 0)
		player->update();

	pathFinding->updateTargetPosition(player->x, player->y);

	for (auto zombie : zombies) {
		zombie->update();

		if (zombie->health <= 0) {
			deleteZombies.push_back(zombie);
		}
	}

	for (auto delProjectile : deleteProjectiles) {
		space->removeDynamicActor(delProjectile);
		projectiles.remove(delProjectile);
		delete delProjectile;
	}

	for (auto delGrenade : deleteGrenades) {
		space->removeDynamicActor(delGrenade);
		grenades.remove(delGrenade);
		delete delGrenade;
	}

	for (auto delZombie : deleteZombies) {
		space->removeDynamicActor(delZombie);
		zombies.remove(delZombie);
		delete delZombie;
	}

	for (auto window : windows) {
		window->update();

		if (window->barrierHealth < WINDOW_BARRIER_MAX_HEALTH && window->isActorInRebuildBarrierRange(player)) {
			showHelpText("Mantén [E] para reparar la barrera");
		}
	}

	for (auto door : doors) {
		if (!door->isOpen && door->isActorInOpenRange(player)) {
			showHelpText("Pulsa [E] para abrir la puerta por " + to_string(doorPrice) + " puntos");
		}
	}

	for (auto weaponLocation : weaponLocations) {
		weaponLocation->update();

		if (weaponLocation->isActorInBuyRange(player)) {
			showHelpText("Pulsa [E] para comprar este arma por " + to_string(weaponLocation->price) + " puntos");
		}
	}

	list<ScoreFloatingText*> deleteScoreFloatingTexts;
	for (auto scoreFloatingText : scoreFloatingTexts) {
		scoreFloatingText->update();

		if (scoreFloatingText->shouldBeDeleted)
			deleteScoreFloatingTexts.push_back(scoreFloatingText);
	}

	for (auto delScoreFloatingText : deleteScoreFloatingTexts) {
		scoreFloatingTexts.remove(delScoreFloatingText);
		delete delScoreFloatingText;
	}

	if (!dead) {
		dead = player->health <= 0;
		if (dead) { // el jugador acaba de morir, prepara la pantalla de muerte y actualiza la puntuación record
			if (score > scoreRecord)
				scoreRecord = score;

			deadScoreText->content = "PUNTUACIÓN: " + to_string(score);
			deadRecordText->content = "RECORD: " + to_string(scoreRecord);

			createBloodSplatter(player->x, player->y);
		}
	}
}

void GameLayer::shakeCamera() {
	cameraShakeTime = 9;
	cameraShakeScrollX = scrollX;
	cameraShakeScrollY = scrollY;
	cameraShakeRadius = 24.0f;
}

void GameLayer::calculateScroll() {
	const float LIMIT_LEFT = 0.48,
				LIMIT_RIGHT = 1.0 - LIMIT_LEFT,
				LIMIT_TOP = 0.48,
				LIMIT_BOTTOM = 1.0 - LIMIT_TOP;

	// limite izquierda
	if (player->x - scrollX < WIDTH * LIMIT_LEFT) {
		scrollX = player->x - WIDTH * LIMIT_LEFT;
		cameraShakeScrollX = scrollX;
	}

	// limite derecha
	if (player->x - scrollX > WIDTH * LIMIT_RIGHT) {
		scrollX = player->x - WIDTH * LIMIT_RIGHT;
		cameraShakeScrollX = scrollX;
	}

	// limite superior
	if (player->y - scrollY < HEIGHT * LIMIT_TOP) {
		scrollY = player->y - HEIGHT * LIMIT_TOP;
		cameraShakeScrollY = scrollY;
	}

	// limite inferior
	if (player->y - scrollY > HEIGHT * LIMIT_BOTTOM) {
		scrollY = player->y - HEIGHT * LIMIT_BOTTOM;
		cameraShakeScrollY = scrollY;
	}

	if (cameraShakeTime > 1) {
		float shakeAngle = (rand() / (float)RAND_MAX) * 2.0f * M_PI; // un ángulo aleatorio en radianes
		float shakeX = cos(shakeAngle) * cameraShakeRadius;
		float shakeY = sin(shakeAngle) * cameraShakeRadius;
		cameraShakeRadius -= 3.0f;

		scrollX = cameraShakeScrollX + shakeX;
		scrollY = cameraShakeScrollY + shakeY;
	}
	else if (cameraShakeTime == 1) {
		// terminar el temblor, restaurar el scroll
		scrollX = cameraShakeScrollX;
		scrollY = cameraShakeScrollY;
	}
}

void GameLayer::draw() {
	SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
	SDL_RenderClear(game->renderer);

	calculateScroll();

	for (auto floor : floorTiles) {
		floor->draw(scrollX, scrollY);
	}
	for (auto blood : bloodSplatters) {
		blood->draw(scrollX, scrollY);
	}
	for (auto wall : wallTiles) {
		wall->draw(scrollX, scrollY);
	}
	for (auto window : windows) {
		window->draw(scrollX, scrollY);
		drawDebugBoundingBox(window, 0, 255, 255);
	}
	for (auto door : doors) {
		door->draw(scrollX, scrollY);
	}
	for (auto weaponLocation : weaponLocations) {
		weaponLocation->draw(scrollX, scrollY);
	}

	if (debugShowPathFindingMap) {
		pathFinding->debugDraw(scrollX, scrollY);
	}

	player->draw(scrollX, scrollY);
	drawDebugBoundingBox(player, 255, 0, 0);

	for (auto zombie : zombies) {
		zombie->draw(scrollX, scrollY);
		drawDebugBoundingBox(zombie, 0, 255, 0);
	}

	for (auto projectile : projectiles) {
		projectile->draw(scrollX, scrollY);
		drawDebugBoundingBox(projectile, 0, 0, 255);
	}

	for (auto grenade : grenades) {
		grenade->draw(scrollX, scrollY);
		drawDebugBoundingBox(grenade, 255, 0, 255);
	}

	// HUD
	for (auto scoreFlyingText : scoreFloatingTexts) {
		scoreFlyingText->draw(scrollX, scrollY);
	}

	if (!dead)
		crosshair->draw();

	healthBar->currentHealth = player->health;
	healthBar->draw();

	scoreText->content = to_string(score) + " PUNTOS";
	scoreText->draw();

	if (rebuildBarrierBarVisible) {
		rebuildBarrierBar->draw(scrollX, scrollY);
	}

	weapon1Icon->weapon = player->weapon1;
	weapon2Icon->weapon = player->weapon2;
	weapon1Icon->draw();
	weapon2Icon->draw();

	if (dead) {
		deadScreenBackground->draw();
		deadText->draw();
		deadScoreText->draw();
		deadRecordText->draw();
		showHelpText("Pulsa [ESPACIO] para reiniciar");
	}
	else if (pause) {
		pauseText->draw();
		showHelpText("Pulsa [P] para reanudar el juego");
	}

	if (!helpText->content.empty()) {
		helpText->draw();
	}

	SDL_RenderPresent(game->renderer); // Renderiza
}

void GameLayer::keysToControls(SDL_Event event) {
	if (event.type == SDL_QUIT) {
		game->loopActive = false;
	}

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
		case SDLK_d: // derecha
			controlMoveX = 1;
			break;
		case SDLK_a: // izquierda
			controlMoveX = -1;
			break;
		case SDLK_w: // arriba
			controlMoveY = -1;
			break;
		case SDLK_s: // abajo
			controlMoveY = 1;
			break;
		case SDLK_q:
			controlSwitchWeapon = true;
			break;
		case SDLK_v:
			controlKnifeAttack = true;
			break;
		case SDLK_e:
			controlRebuildBarrier = true;
			controlBuy = true;
			break;
		case SDLK_p:
			controlPause = true;
			break;
		case SDLK_F1:
			controlDebugToggleBoundingBoxes = true;
			break;
		case SDLK_F2:
			controlDebugTogglePathfindingMap = true;
			break;
		case SDLK_F3:
			controlDebugAddScore = true;
			break;
		case SDLK_SPACE:
			controlDeadRestart = true;
			break;
		}


	}
	if (event.type == SDL_KEYUP) {
		int code = event.key.keysym.sym;
		// Levantada
		switch (code) {
		case SDLK_d: // derecha
			if (controlMoveX == 1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_a: // izquierda
			if (controlMoveX == -1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_w: // arriba
			if (controlMoveY == -1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_s: // abajo
			if (controlMoveY == 1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_e:
			controlRebuildBarrier = false;
			break;
		}

	}

}

void GameLayer::mouseToControls(SDL_Event event) {
	// Cada vez que hacen click
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			controlShoot = true;
		}
	}
	// Cada vez que se mueve
	if (event.type == SDL_MOUSEMOTION) {
		controlAimX = event.motion.x / game->scaleLower;
		controlAimY = event.motion.y / game->scaleLower;
	}
	// Cada vez que levantan el click
	if (event.type == SDL_MOUSEBUTTONUP) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			controlShoot = false;
		}
	}
}

void GameLayer::loadMap(const string& name) {
	char character;
	string line;
	ifstream streamFile(name.c_str());
	if (!streamFile.is_open()) {
		cout << "Falla abrir el fichero de mapa" << endl;
		return;
	}
	else {
		vector<string> map;
		for (int i = 0; getline(streamFile, line); i++) {
			mapWidth = line.length() * TILE_WIDTH; // Ancho del mapa en pixels
			map.push_back(line);
		}
		mapHeight = map.size() * TILE_HEIGHT;

		// Por línea
		for (int i = 0; i < map.size(); i++) {
			const string& row = map[i];
			// Por carácter (en cada línea)
			for (int j = 0; j < row.size(); j++) {
				character = row[j]; // Leer character
				loadMapObject(character, j, i, map);
			}
		}

		pathFinding = new PathFinding(map, game);
		pathFinding->updateTargetPosition(player->x / (float)TILE_WIDTH, player->y / (float)TILE_HEIGHT);
	}
	streamFile.close();
}

static int getTileNeighbours(int x, int y, const vector<string>& map, bool wall)
{
	int neighbours = 0;
	const int neighboursOffsetX[TN_NUM_NEIGHBOURS]{  0, -1, 1, 0, -1,  1, -1, 1 };
	const int neighboursOffsetY[TN_NUM_NEIGHBOURS]{ -1,  0, 0, 1, -1, -1,  1, 1 };
	const int neighboursPrerequisites[TN_NUM_NEIGHBOURS]{ 0, 0, 0, 0, TN_NORTH | TN_WEST, TN_NORTH | TN_EAST, TN_SOUTH | TN_WEST, TN_SOUTH | TN_EAST };
	for (int i = 0; i < TN_NUM_NEIGHBOURS; i++) {
		int neighbourX = x + neighboursOffsetX[i];
		int neighbourY = y + neighboursOffsetY[i];
		if (neighbourY >= 0 && neighbourY < map.size() && // dentro del mapa?
			neighbourX >= 0 && neighbourX < map[neighbourY].size() &&
			(neighbours & neighboursPrerequisites[i]) == neighboursPrerequisites[i] && // y tiene las conexiones necesarias?
			(wall && map[neighbourY][neighbourX] == '#' || !wall && map[neighbourY][neighbourX] != '#')) { // y es del mismo tipo?
			neighbours |= 1 << i;
		}
	}
	return neighbours;
}

void GameLayer::loadMapObject(char character, int mapX, int mapY, const vector<string>& map)
{
	bool createFloorTile = true;

	float x = TILE_WIDTH / 2 + mapX * TILE_WIDTH; // x central
	float y = TILE_HEIGHT + mapY * TILE_HEIGHT; // y suelo
	switch (character) {
	case 'P': {
		player = new Player(x, y, game);
		player->y = player->y - player->height / 2;
		space->addDynamicActor(player);
		break;
	}
	case 'Z': {
		Zombie* zombie = new Zombie(x, y, game);
		zombie->y = zombie->y - zombie->height / 2;
		zombies.push_back(zombie);
		space->addDynamicActor(zombie);
		break;
	}
	case '1': {
		WeaponLocation* l = new WeaponLocation(WeaponType::Pistol, 20, 400, x, y, game);
		l->y = l->y - l->height / 2;
		weaponLocations.push_back(l);
		break;
	}
	case '2': {
		WeaponLocation* l = new WeaponLocation(WeaponType::Shotgun, 15, 800, x, y, game);
		l->y = l->y - l->height / 2;
		weaponLocations.push_back(l);
		break;
	}
	case '3': {
		WeaponLocation* l = new WeaponLocation(WeaponType::GrenadeLauncher, 5, 1200, x, y, game);
		l->y = l->y - l->height / 2;
		weaponLocations.push_back(l);
		break;
	}
	case 'W': {
		bool horizontal = mapX >= 1 ? map[mapY][mapX - 1] == '#' : false;
		Window* window = new Window(horizontal, x, y, game);
		window->y = window->y - window->height / 2;
		if (horizontal)
			window->x += TILE_WIDTH / 2;
		windows.push_back(window);
		space->addStaticActor(window);
		createFloorTile = false;
		break;
	}
	case 'D': {
		bool horizontal = mapX >= 1 ? map[mapY][mapX - 1] == '#' : false;
		Door* door = new Door(horizontal, x, y, game);
		door->y = door->y - door->height / 2;
		if (horizontal)
			door->x += TILE_WIDTH / 2;
		doors.push_back(door);
		space->addStaticActor(door);
		break;
	}
	case '#': {
		Tile* tile = new Tile("res/level/wall_bricks.png", getTileNeighbours(mapX, mapY, map, true), x, y, game);
		tile->y = tile->y - tile->height / 2;
		wallTiles.push_back(tile);
		space->addStaticActor(tile);
		createFloorTile = false;
		break;
	}
	}

	if (createFloorTile) {
		Tile* tile = new Tile("res/level/floor_stone.png", getTileNeighbours(mapX, mapY, map, false), x, y, game);
		tile->y = tile->y - tile->height / 2;
		floorTiles.push_back(tile);
	}
}

void GameLayer::createProjectile(float x, float y, float vx, float vy, int damage, int hitScore, int timeToLive, bool isBullet, bool fromZombie) {
	Projectile* p = new Projectile(x, y, vx, vy, damage, hitScore, timeToLive, isBullet, fromZombie, game);
	space->addDynamicActor(p);
	projectiles.push_back(p);
}

void GameLayer::createGrenade(float x, float y, float vx, float vy, int damage, int timeToExplode) {
	Grenade* g = new Grenade(x, y, vx, vy, damage, timeToExplode, game);
	space->addDynamicActor(g);
	grenades.push_back(g);
}

void GameLayer::createZombie(float x, float y) {
	Zombie* zombie = new Zombie(x, y, game);
	space->addDynamicActor(zombie);
	zombies.push_back(zombie);
}

void GameLayer::createBloodSplatter(float x, float y) {
	BloodSplatter* blood = new BloodSplatter(x, y, game);
	bloodSplatters.push_back(blood);
}

void GameLayer::addScore(int scorePoints, float x, float y) {
	score += scorePoints;
	scoreFloatingTexts.push_back(new ScoreFloatingText(x, y, scorePoints, game));
}

void GameLayer::showHelpText(const string& helpTextString) {
	helpText->content = helpTextString;
}

void GameLayer::drawDebugBoundingBox(Actor* actor, int r, int g, int b) {
	if (debugShowBoundingBoxes) {
		SDL_Rect boundingBox{};
		boundingBox.x = actor->x - scrollX - actor->width / 2.0f;
		boundingBox.y = actor->y - scrollY - actor->height / 2.0f;
		boundingBox.w = actor->width;
		boundingBox.h = actor->height;
		SDL_SetRenderDrawColor(game->renderer, r, g, b, 255);
		SDL_RenderDrawRect(game->renderer, &boundingBox);
	}
}
