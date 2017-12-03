#pragma once
#include <SFML/Network.hpp>
#include <list>
#include <sstream>
#include <iostream>
#include "Player.h"
#include <queue>
using namespace sf;

class Listener
{
public:
	TcpSocket socket_server_listen;
	static Listener* getInstance()
	{
		if (instance == nullptr)
		{
			instance = new Listener();
		}
		return instance;
	}
	static void release()
	{
		delete instance;
		instance = nullptr;
	}
	void fetch_position_from_server(float position[])
	{
		socket_server_listen.setBlocking(false);
		Socket::Status is_connected = Socket::Done;
		while (true)
		{

		}

	}
	void listening_Server(float position[], bool players_Connected[], std::map<int, Player*> &list_player, int &nb_player_connected,Player player_list[])
	{
		int id_player_connected[11];
		bool first_connection = false;
		int player = 0;
		int player_id = 0;
		int ptr = 4;
		bool init_nb_player = false;
		socket_server_listen.setBlocking(false);
		//std::cout << "NON BLOCK\n";
		char data[2000];
		data[0] = 'o';
		data[1] = 'k';
		std::size_t received;
		std::string number = "";
		std::string test;
		std::stringstream ss;
		std::string num_to_int = "";
		int id = 0;
		int index = 0;
		Socket::Status is_connected = Socket::Done;
		
		while (is_connected != Socket::Disconnected)
		{
			player = 0;
			ptr = 4;
			ss.str("");
			//memset(data, 0, 2000);
			is_connected = socket_server_listen.receive(data, 2000, received);
			sleep(milliseconds(1));
			if (data[0] == 'I')
			{
				ss << data[2];
				num_to_int = ss.str();
				ss.str("");
				if (stoi(num_to_int) !=0)
				{
					ss << data[1] << data[2];
					num_to_int = ss.str();
				}
				else
				{
					ss << data[1];
					num_to_int = ss.str();
				}
				if (stoi(num_to_int) !=19)
				{
					player_id = stoi(num_to_int);
				}
				id_player_connected[0] = player_id;
				//info du joueur.
				Player* player_null = nullptr;
				list_player.insert_or_assign(player_id, player_null);
				std::cout << "ID : " << player_id;
				//std::cout << "Initialisation data";
				while (data[ptr] != '&')
				{
					char add_nb = data[ptr];
					if (add_nb != '|')
					{
						number += add_nb;
					}
					else
					{
						if (!init_nb_player)
						{
							nb_player_connected = stoi(number);
							init_nb_player = true;
							//std::cout << "CONNECT :" << nb_player_connected << std::endl;
						}
						else
						{
							id = stoi(number);
							if (id !=999)
							{
								if (id != player_id)
								{
									
									for (int i=1;i<11;i++)
									{
										if (id_player_connected[i] != id)
										{
											std::cout << id << " = " << id_player_connected[player];
											//std::cout << id << " = " << player_id << std::endl;
											first_connection = true;
										}
									}
									if (first_connection)
									{
										player++;
										std::cout << id << " = " << player_id << std::endl;
										player_list[player].set_active(true);
										list_player.insert_or_assign(id, &player_list[player]);
										id_player_connected[player] = id;
										first_connection = false;
										std::cout << player << std::endl;
									}
								}
							}
							//std::cout << "ID :" << id << std::endl;
							
						}
						number = "";
					}
					ptr++;
				}
				//std::cout << player_list[0].getPosition().x<<std::endl<< player_list[0].getPosition().x << std::endl;
				memset(data, 0, 2000);
			}
		}
		//std::cout << "Server unreachable !\n";
	}
private:
	static Listener* instance;
	Listener()
	{
		
	}
	//01|#1|45|12|1|1|?1|@0045|&
	
};
Listener* Listener::instance = nullptr;
