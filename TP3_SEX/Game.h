#pragma once

#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Player.h"

using namespace sf;

class Game
{
public:
	Game();
	~Game();
	int run();

	void Game::open_Connection(bool &statut, bool &thread_stat);
private:
	const int WIDTH = 1280;
	const int HEIGHT = 720;

	bool init();
	void getInputs();
	void update();
	void draw();

	Player *player1;
	Player *player2;

	Texture player1T;
	Texture player2T;

	Sprite background;
	Texture backgroundT;

	RenderWindow mainWin;
	View view;
	Event event;
	float position[20] = { 0 };
	bool player_connected[11] = { false };
	bool connected = false;
	bool finished = true;
};