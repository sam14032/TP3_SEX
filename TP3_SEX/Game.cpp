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

}

int Game::run()
{
	sf::Thread check_server([&] {Game::open_Connection(std::ref(connected), std::ref(finished), std::ref(list_of_players), std::ref(player_list)); });
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
	Vector2f pos{ 50,400 };
	player1.init(pos, player1T);
	for (int i=0; i<9; i++)
	{
		pos = {(float) (50 + 50 * i),400 };
		player_list[i].init(pos, player1T);
	}
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
		player1.update(WIDTH, HEIGHT,std::ref(Listener::getInstance()->socket_server_listen));
	}
}

void Game::draw()
{
	mainWin.clear();
	if (connected)
	{
		
		mainWin.draw(background);
		int cpt = 0;
		for (int i=0; i<9; i++)
		{
			if (player_list[i].is_active())
			{
				cpt++;
				player_list[i].draw(mainWin);
			}
		}
		player1.draw(mainWin);
		//std::cout << cpt;
	}
	else
	{
		mainWin.draw(connection_stat);
	}
	//player2->draw(mainWin);

	mainWin.display();
}

void Game::open_Connection(bool &statut, bool &thread_stat, std::map<int, Player*> &players_list, Player players[])
{
	bool connected = false;
	thread_stat = false;
	Time time_max = sf::seconds(1);
	Listener* server_listen = Listener::getInstance();
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
				stat = server_listen->socket_server_listen.connect("10.200.23.90", 5000, time_max);
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
			if (!connected)
			{
				statut = true;
				std::cout << "CONNECTION !\n";
				server_listen->listening_Server(position, player_connected, std::ref(players_list), std::ref(nb_players_connected),std::ref(players),player1);
				connected = true;
			}
			
		}
		thread_stat = true;
	}

}