#pragma once
#include <SFML/Network.hpp>
#include <list>
#include <queue>
#include <sstream>
#include <iostream>
using namespace sf;
class Listener
{
public:
	//01|#1|45|12|1|1|?1|@0045|&
	TcpSocket socket_server_listen;
	void fetch_position_from_server(float position[])
	{
		socket_server_listen.setBlocking(false);
		Socket::Status is_connected = Socket::Done;
		while (true)
		{

		}

	}
	void listening_Server(float position[], bool players_Connected[])
	{
		//Socket::Status status = socket_server_listen.connect("192.168.79.109", 8000);
		socket_server_listen.setBlocking(false);
		//std::cout << "NON BLOCK\n";
		char data[2000];
		data[0] = 'o';
		data[1] = 'k';
		std::size_t received;
		std::string number = "";
		std::string test;
		int index = 0;
		Socket::Status is_connected = Socket::Done;

		while (is_connected != Socket::Disconnected)
		{
			//memset(data, 0, 2000);
			is_connected = socket_server_listen.receive(data, 2000, received);
			char nb_Joueur[2];
			static bool good = true;
			std::cin >> test;
			for (int i = 0; i<test.length(); i++)
			{
				if (good)
				{
					memset(data, 0, 2000);
				}
				good = false;
				data[i] = test[i];
			}
			std::cout << "Is connected !\n";
			std::cout << "Sending...\n";
			socket_server_listen.send(data, 2000);
			good = false;
		}
		std::cout << "Server unreachable !\n";
	}
};