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

	Player player_list[9];
	void Game::open_Connection(bool &statut, bool &thread_stat, std::map<int, Player*> &players_list,Player players[]);
	int nb_players_connected = 0;
private:
	const int WIDTH = 1280;
	const int HEIGHT = 720;

	bool init();
	void getInputs();
	void update();
	void draw();
	std::map<int, Player*> list_of_players;
	Player player1;
	Texture player1T;

	Sprite background;
	Texture backgroundT;

	RenderWindow mainWin;
	View view;
	Event event;
	float position[20] = { 0 };
	bool player_connected[11] = { false };
	bool connected = false;
	bool finished = true;
	Font font;
	Text connection_stat;
};