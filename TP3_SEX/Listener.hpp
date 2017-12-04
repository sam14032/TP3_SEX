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
		socket_server_listen.setBlocking(false);
		Socket::Status is_connected = Socket::Done;
		
		while (is_connected != Socket::Disconnected)
		{
			is_connected = socket_server_listen.receive(data_fetched_from_server, 2000, received);
			sleep(milliseconds(1));

			if (data_fetched_from_server[0] == 'I')
			{
				update_client_view(std::ref(data_fetched_from_server), std::ref(list_player),
					std::ref(nb_player_connected), std::ref(player_list));
			}
		}
	}

	// if data received from server start with I then update the client current view by adding player.
	void update_client_view(char data_fetched_from_server[],std::map<int, Player*> &list_player, int &nb_player_connected, Player player_list[])
	{
		static bool first_connection = true;
		int current_player_iterator = 0;
		static int player_id = 777;
		int position_in_the_data_string = 4;
		bool init_nb_player = false;

		int other_player_id = 0;
		std::size_t received;
		//char streamed in a string to convert to int
		std::string number = "";
		//stringstream
		std::stringstream ss;
		//string to convert and get the id.
		std::string num_to_int = "";
		//id assigné au joueur.
		//check the 3rd data byte and convert to int which give the player id.
		//if player_id not initialized, assign data.
		if (player_id == 777)
		{
			ss << data_fetched_from_server[2];
			num_to_int = ss.str();
			ss.str("");
			if (stoi(num_to_int) != 0)
			{
				ss << data_fetched_from_server[1] << data_fetched_from_server[2];
				num_to_int = ss.str();
			}
			else
			{
				ss << data_fetched_from_server[1];
				num_to_int = ss.str();
			}
			//store the current player's id in the first place of the array.
			player_id = stoi(num_to_int);
			id_player_connected[0] = player_id;
			new_id[0] = player_id;
			old_id[0] = player_id;
			//info du joueur.
			//add the current player's instance and id in the watch list.
			Player* player_null = nullptr;
			list_player.insert_or_assign(player_id, player_null);
		}
		std::cout << data_fetched_from_server << std::endl;
		//check the data at specific position.
		while (data_fetched_from_server[position_in_the_data_string] != '&')
		{
			char current_char = data_fetched_from_server[position_in_the_data_string];
			//check if the current_char is a data separator.
			if (current_char != '|')
			{
				number += current_char;
			}
			//if separator, convert the data we've stored in the string to a int.
			else
			{
				// init the number of player everytime this method is called.
				if (!init_nb_player)
				{
					nb_player_connected = stoi(number);
					init_nb_player = true;
				}
				else
				{
					//convert the received data to a int id.
					other_player_id = stoi(number);
					// if the data is not equal to junk.
					if (other_player_id != 999)
					{
						std::cout << "Other 1 :" << other_player_id << std::endl;
						//std::cout << other_player_id << " = " << player_id << std::endl;
						//check if the data isn't the current player id.
						if (other_player_id != player_id)
						{
							std::cout << "Current id : " << player_id << std::endl;
							//reset the check to true every loop. (state where its the first other player's connection)
							first_connection = true;
							for (int i = 1; i<11; i++)
							{
								//check through all the current ids in the client's system.
								if (id_player_connected[i] == other_player_id)
								{
									std::cout << "ID from server : "<<other_player_id <<std::endl << "ID from client : "<< id_player_connected[i] <<std::endl;
									first_connection = false;
								}
							}
							//if not in the system, add to client's system.
							if (first_connection)
							{
								std::cout << "ID other : " << other_player_id << std::endl;
								//add one player to the player's count.
								current_player_iterator++;
								std::cout << "Good loop : "<< current_player_iterator << std::endl;
								//add one active player to the active player watch list.
								player_list[current_player_iterator].set_active(true);
								//add the id of the player and create a new player's instance.
								list_player.insert_or_assign(other_player_id, &player_list[current_player_iterator]);
								//add the current other player's id to the player's id watch list.
								id_player_connected[current_player_iterator] = other_player_id;
								new_id[current_player_iterator] = other_player_id;
								old_id[current_player_iterator] = other_player_id;
							}
						}//end of current checked player id != current id
						else
						{
							//std::cout << "egal" << std::endl;
						}
					}
					//else
					//{
					//	//add one player to the player's count.
					//	current_player_iterator++;
					//	std::cout << "Other : " << other_player_id << std::endl;
					//	//std::cout << "Bad loop :" << current_player_iterator << std::endl;
					//	new_id[current_player_iterator] = other_player_id;
					//}
					//end of junk check
				} //end check if init.
				//reset the string's data container to empty. (avoid residual memory junk).
				number = "";
			}
			//incremente the iterator to move in the data's array.
			position_in_the_data_string++;
		}
		//check if someone went offline.
		//for (int i=1;i<11;i++)
		//{
		//	//check if data is initialized or not.
		//	//if init, check if old and new id is the same : else, change information to offline.
		//	if (old_id[i] != 999 && old_id[i] != new_id[i])
		//	{
		//		std::cout << i;
		//		player_list[i].set_active(false);
		//	}
		//	old_id[i] = new_id[i];
		//}
		memset(data_fetched_from_server, 0, 2000);
	}
private:
	static Listener* instance;
	int id_player_connected[11];
	int new_id[11] = { 999,999, 999, 999, 999, 999, 999, 999, 999, 999, 999 };;
	int old_id[11] = {999,999, 999, 999, 999, 999, 999, 999, 999, 999, 999};
	char data_fetched_from_server[2000];
	size_t received;
	Listener()
	{
		
	}
	//01|#1|45|12|1|1|?1|@0045|&
	
};
Listener* Listener::instance = nullptr;


