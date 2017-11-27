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
	TcpSocket socket_Listen;
	void listening_Server(float position[],bool players_Connected[])
	{
		Socket::Status status = socket_Listen.connect("192.168.79.129", 8888);
		socket_Listen.setBlocking(false);
		std::cout << "NON BLOCK\n";
		std::deque<char> data[2000];
		std::size_t received;
		std::string number = "";
		int index = 0;
		Socket::Status is_connected = Socket::Done;
		
		while (is_connected == Socket::Done)
		{
			std::cout << "Is connected !\n";
			memset(data, 0, 2000);
			is_connected = socket_Listen.receive(data, 2000, received);
			char nb_Joueur[2];
			//retreive data sent from the server.
			char nb_user1 = data->front();
			if (nb_user1 != NULL)
			{
				std::deque<char>::iterator it = std::find(data->begin(), data->end(), '&');
				while (it != data->end())
				{
					while (data->front() != '|')
					{
						char add_nb = data->front();
						number += add_nb;
						data->pop_front();
					}
					position[index] = atof(number.c_str());
					index++;
					data->pop_front();
					if (data->front() == '&')
					{
						data->pop_front();
						it = std::find(data->begin(), data->end(), '&');
					}
				}
			}

		}
	}
};