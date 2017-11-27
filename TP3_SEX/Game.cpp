#include "Game.h"
#include <iostream>
#include "Listener.hpp"
#include "Communication.h"
Game::Game()
{
	mainWin.create(VideoMode(WIDTH, HEIGHT, 32), "LE jeu"); 
	view = mainWin.getDefaultView();

	mainWin.setVerticalSyncEnabled(true);
}

Game::~Game()
{
	delete player1;
	//delete player2;
}

int Game::run()
{
	sf::Thread check_server([&] {Game::open_Connection(std::ref(connected), std::ref(finished)); });
	if (!init())
	{
		return EXIT_FAILURE;
	}

	while (mainWin.isOpen())
	{
		static int buf = 21;

		if (!connected)
		{
			if (buf >3 && finished)
			{
				std::cout << "here\n";
				check_server.launch();
			}
		}
		buf++;
		getInputs();
		update();
		draw();
	}

	return EXIT_SUCCESS;
}

bool Game::init()
{
	if (!player1T.loadFromFile("Sprites\\player1.png"))
	{
		return false;
	}
	/*if (!player2T.loadFromFile("Sprites\\player2.png"))
	{
		return false;
	}*/
	if (!backgroundT.loadFromFile("Sprites\\background.jpg"))
	{
		return false;
	}

	background.setTexture(backgroundT);
	background.setPosition(0, 0);

	player1 = new Player(Vector2f(WIDTH / 2, HEIGHT / 2), player1T);
	//player2 = new Player(Vector2f(WIDTH / 2, HEIGHT / 2), player2T);

	return true;
}

void Game::getInputs()
{
	while (mainWin.pollEvent(event))
	{
		if (event.type == Event::Closed)
		{
			mainWin.close();
		}
	}
}

void Game::update()
{
	player1->update(WIDTH, HEIGHT);
	//player2->update(WIDTH, HEIGHT);
}

void Game::draw()
{
	mainWin.clear();

	mainWin.draw(background);
	player1->draw(mainWin);
	//player2->draw(mainWin);

	mainWin.display();
}

void Game::open_Connection(bool &statut, bool &thread_stat)
{
	thread_stat = false;
	Time time_max = sf::seconds(1);
	Listener server_listen;
	int time_out = 0;
	Socket::Status stat = Socket::Error;
	while (stat != Socket::Done && time_out < 10)
	{
		stat = server_listen.socket_Listen.connect("192.168.79.129", 8888, time_max);
		if (stat == Socket::Done)
		{
			time_out = 12;
		}
		std::cout << "Connection : " << stat << std::endl;
		std::cout << "try : " << time_out << std::endl;
		time_out++;
	}
	if (time_out >= 10)
	{
		std::cout << "Server unreachable\n";
	}
	else if (stat == Socket::Done)
	{
		statut = true;
		std::cout << "CONNECTION !\n";
		server_listen.listening_Server(position, player_connected);

	}
	thread_stat = true;
}