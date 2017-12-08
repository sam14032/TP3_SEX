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
	
	void listening_Server(float position[], bool players_Connected[], std::map<int, Player*> &list_player, int &nb_player_connected,Player player_list[],Player &player1,char* data_to_send)
	{
		socket_server_listen.setBlocking(false);
		Socket::Status is_connected = Socket::Done;
		static int buffer = 0;
		/*char data[2];
		data[0] = 'O';
		data[1] = 'K';
		socket_server_listen.send(data, 2);*/
		while (is_connected != Socket::Disconnected)
		{
			is_connected = socket_server_listen.receive(data_fetched_from_server, 2000, received);
			sleep(milliseconds(1));
			/*if (is_connected == Socket::Done)
			{
				std::cout << data_fetched_from_server << std::endl;
			}*/
			if (data_fetched_from_server[0] == 'C')
			{
				std::cout << data_fetched_from_server << std::endl;
				update_client_view(std::ref(data_fetched_from_server),
					std::ref(nb_player_connected), std::ref(player_list));
			}
			if (data_fetched_from_server[0] == 'I')
			{
				std::cout << data_fetched_from_server << std::endl;
				init_own_client_data(std::ref(data_fetched_from_server),
					std::ref(nb_player_connected), std::ref(player1));
			}
			if (data_fetched_from_server[0] == 'D')
			{
				delete_other_client(std::ref(data_fetched_from_server),
					std::ref(nb_player_connected), std::ref(player_list));
			}
			if (data_fetched_from_server[0] == 'U')
			{
				std::cout << data_fetched_from_server << std::endl;
				update_view_with_player_already_on(std::ref(data_fetched_from_server),
					std::ref(nb_player_connected), std::ref(player_list));
			}
			if (player1.input && buffer%3 ==0)
			{
				char* data = player1.format_data_to_send(player1.get_id(), player1.getPosition(),
					player1.active_command.direction);
				Socket::Status stat = socket_server_listen.send(data, 2000);
				buffer = 0;
				
			}
			if (data_fetched_from_server[0] == 'P')
			{
				std::cout << data_fetched_from_server << std::endl;
				update_other_player_position(std::ref(data_fetched_from_server), std::ref(player_list));
			}
			buffer++;
			if (buffer >3)
			{
				buffer = 0;
			}
		}
	}


	// if data received from server start with I then update the client current view by adding player.
	void update_client_view(char data_fetched_from_server[], int &nb_player_connected, Player player_list[])
	{
		nb_player_connected++;
		int current_instance_player = 0;
		for (int i=0; i<9; i++)
		{
			if (!player_list[i].is_active())
			{
				current_instance_player = i;
				player_list[i].set_active(true);
				break;
			}
		}
		int position_in_the_data_string = 1;
		int other_player_id = 0;
		int data_formatted = 0;
		//char streamed in a string to convert to int
		std::string number = "";
		//id assigné au joueur.
		//check the 3rd data byte and convert to int which give the player id.
		//if player_id not initialized, assign data.list_player.insert_or_assign(player_id, &player1);
		//check the data at specific position.
		while (data_fetched_from_server[position_in_the_data_string] != '&')
		{
			char current_char = data_fetched_from_server[position_in_the_data_string];
			//check if the current_char is a data separator.
			if (current_char != '|')
			{
				number += current_char;
			}
			else
			{
				//if separator, convert the data we've stored in the string to a int.
				switch (data_formatted)
				{
				case 0:
					other_player_id = stoi(number);
					player_list[current_instance_player].set_id(other_player_id);
					data_formatted++;
					number = "";
					break;
				case 1:
					player_list[current_instance_player].setPosX(std::stof(number));
					std::cout << player_list[current_instance_player].getPosition().x << std::endl;
					data_formatted++;
					number = "";
					break;
				case 2:
					player_list[current_instance_player].setPosY(std::stof(number));
					std::cout << player_list[current_instance_player].getPosition().y << std::endl;
					data_formatted++;
					number = "";
					break;
				}
			}
			
			position_in_the_data_string++;
		}
		player_list[current_instance_player].set_drawing_position();
		memset(data_fetched_from_server, 0, 2000);

	}
	void init_own_client_data(char data_fetched_from_server[], int &nb_player_connected,Player &player1)
	{
		int position_in_the_data_string = 1;
		int own_id = 0;
		int data_formatted = 0;
		//char streamed in a string to convert to int
		std::string number = "";
		//id assigné au joueur.
		//check the 3rd data byte and convert to int which give the player id.
		//if player_id not initialized, assign data.list_player.insert_or_assign(player_id, &player1);
		//check the data at specific position.
		while (data_fetched_from_server[position_in_the_data_string] != '&')
		{
			char current_char = data_fetched_from_server[position_in_the_data_string];
			//check if the current_char is a data separator.
			if (current_char != '|')
			{
				number += current_char;
			}
			else
			{
				//if separator, convert the data we've stored in the string to a int.
				switch (data_formatted)
				{
				case 0:
					own_id = stoi(number);
					player1.set_id(own_id);
					data_formatted++;
					number = "";
					break;
				case 1:
					player1.setPosX(std::stof(number));
					data_formatted++;
					number = "";
					break;
				case 2:
					player1.setPosY(std::stof(number));
					data_formatted++;
					number = "";
					break;
				}
			}
			position_in_the_data_string++;
		}
		memset(data_fetched_from_server, 0, 2000);
	}

	void delete_other_client(char data_fetched_from_server[], int &nb_player_connected, Player player_list[])
	{

		nb_player_connected--;
		int current_instance_player = 0;
		int position_in_the_data_string = 1;
		int other_player_id = 0;
		int data_formatted = 0;
		//char streamed in a string to convert to int
		std::string number = "";
		//id assigné au joueur.
		//check the 3rd data byte and convert to int which give the player id.
		//if player_id not initialized, assign data.list_player.insert_or_assign(player_id, &player1);
		//check the data at specific position.
		while (data_fetched_from_server[position_in_the_data_string] != '&')
		{
			char current_char = data_fetched_from_server[position_in_the_data_string];
			//check if the current_char is a data separator.
			if (current_char != '|')
			{
				number += current_char;
			}
			else
			{
				other_player_id = stoi(number);
			}

			position_in_the_data_string++;
		}
		for (int i=0;i<9;i++)
		{
			if (player_list[i].get_id()== other_player_id)
			{
				player_list[i].set_active(false);
				break;
			}
		}
		memset(data_fetched_from_server, 0, 2000);
	}

	void update_view_with_player_already_on(char data_fetched_from_server[], int &nb_player_connected, Player player_list[])
	{
		int current_instance_player = 0;
		
		int position_in_the_data_string = 1;
		int other_player_id = 0;
		int data_formatted = 0;
		//char streamed in a string to convert to int
		std::string number = "";
		//id assigné au joueur.
		//check the 3rd data byte and convert to int which give the player id.
		//if player_id not initialized, assign data.list_player.insert_or_assign(player_id, &player1);
		//check the data at specific position.
		while (data_fetched_from_server[position_in_the_data_string] != '&')
		{
			char current_char = data_fetched_from_server[position_in_the_data_string];
			//check if the current_char is a data separator.
			if (current_char != '|')
			{
				number += current_char;
			}
			else
			{
				//if separator, convert the data we've stored in the string to a int.
				switch (data_formatted)
				{
				case 0:
					for (int i = 0; i<9; i++)
					{
						if (!player_list[i].is_active())
						{
							player_list[i].set_active(true);
							current_instance_player = i;
							nb_player_connected++;
							break;
						}
					}

					other_player_id = stoi(number);
					player_list[current_instance_player].set_id(other_player_id);
					data_formatted++;
					number = "";
					break;
				case 1:
					player_list[current_instance_player].setPosX(std::stof(number));
					std::cout << player_list[current_instance_player].getPosition().x << std::endl;
					data_formatted++;
					number = "";
					break;
				case 2:
					player_list[current_instance_player].setPosY(std::stof(number));
					std::cout << player_list[current_instance_player].getPosition().y << std::endl;
					data_formatted++;
					number = "";
					break;
				}
				if (data_formatted ==3)
				{
					data_formatted = 0;
				}
			}

			position_in_the_data_string++;
		}
		player_list[current_instance_player].set_drawing_position();
		memset(data_fetched_from_server, 0, 2000);
	}

	void update_other_player_position(char data_fetched_from_server[], Player player_list[])
	{
		int current_instance_player = 0;
		int position_in_the_data_string = 1;
		int other_player_id = 0;
		int data_formatted = 0;
		Vector2f position;
		//char streamed in a string to convert to int
		std::string number = "";
		//id assigné au joueur.
		//check the 3rd data byte and convert to int which give the player id.
		//if player_id not initialized, assign data.list_player.insert_or_assign(player_id, &player1);
		//check the data at specific position.
		while (data_fetched_from_server[position_in_the_data_string] != '&')
		{
			char current_char = data_fetched_from_server[position_in_the_data_string];
			//check if the current_char is a data separator.
			if (current_char != '|')
			{
				number += current_char;
			}
			else
			{
				if (data_formatted==0)
				{
					other_player_id = stoi(number);
				}
				if (data_formatted ==1)
				{
					position.x = stof(number);
				}
				if (data_formatted == 2)
				{
					position.y = stof(number);
				}
				data_formatted++;
				number = "";
			}

			position_in_the_data_string++;
		}
		for (int i = 0; i<9; i++)
		{
			if (player_list[i].get_id() == other_player_id)
			{
				std::cout << i;
				player_list[i].setPosX(position.x);
				player_list[i].setPosY(position.y);
				player_list[i].set_drawing_position();
				break;
			}
		}
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


