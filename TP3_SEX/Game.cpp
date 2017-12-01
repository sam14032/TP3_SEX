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
	if (!font.loadFromFile("Ressources\\Fonts\\Peric.ttf"))
	{
		return false;
	}
	connection_stat.setColor(sf::Color::White);
	connection_stat.setCharacterSize(30);
	connection_stat.setFont(font);
	connection_stat.setString("No connection");
	connection_stat.setPosition(20, 360);


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
	if (connected)
	{
		player1->update(WIDTH, HEIGHT);
	}
}

void Game::draw()
{
	mainWin.clear();
	if (connected)
	{
		mainWin.draw(background);
		player1->draw(mainWin);
	}
	else
	{
		mainWin.draw(connection_stat);
	}
	//player2->draw(mainWin);

	mainWin.display();
}

void Game::open_Connection(bool &statut, bool &thread_stat)
{
	thread_stat = false;
	Time time_max = sf::seconds(1);
	Listener server_listen;
	int time_out = 0;
	std::string try_again = "";
	Socket::Status stat = Socket::Error;
	while (try_again != "n")
	{
		while (stat != Socket::Done)
		{
			if (time_out < 10)
			{
				//10.200.23.90 cegep ste-foy
				//maison 192.168.11.109
				stat = server_listen.socket_server_listen.connect("192.168.11.109", 8000, time_max);
				if (stat == Socket::Done)
				{
					time_out = 12;
				}
				std::cout << "Connection : " << stat << std::endl;
				std::cout << "try : " << time_out << std::endl;
				time_out++;
			}
			else
			{
				time_out = 999;
				break;
			}

		}
		if (time_out == 999)
		{
			std::cout << "Server unreachable\n";
			std::cout << "Try again to connect (Y/N) : ";
			std::cin >> try_again;
			if (try_again == "y")
			{
				time_out = 0;
			}
		}
		else if (stat == Socket::Done)
		{
			statut = true;
			std::cout << "CONNECTION !\n";
			server_listen.listening_Server(position, player_connected);

		}
		thread_stat = true;
	}

}