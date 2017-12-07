#include "Player.h"
#include <string>;
#include <iomanip>
#include <sstream>;
#include <iostream>
using namespace std;
Player::Player()
{
}


Player::~Player()
{

}

void Player::update(const float &widthLimit, const float &heightLimit)
{
	input = getInputs(widthLimit, heightLimit);

}

void Player::draw(RenderWindow &window)
{
	window.draw(spritePlayer);
}

Vector2f Player::getPosition() const
{
	return active_command.pos_now;
}

void Player::set_drawing_position()
{
	spritePlayer.setPosition(active_command.pos_now);
}

void Player::setPosX(const float &newPosX)
{
	active_command.pos_now.x = newPosX;
}

void Player::setPosY(const float &newPosY)
{
	active_command.pos_now.y = newPosY;
}

void Player::init(const Vector2f position, Texture & texture)
{
	this->active_command.pos_now = position;

	spritePlayer.setTexture(texture);
	spritePlayer.setScale(0.2f, 0.2f);
	spritePlayer.setOrigin(spritePlayer.getGlobalBounds().width / 2, spritePlayer.getGlobalBounds().height / 2);
}

void Player::set_active(bool stat)
{
	active = stat;
}

bool Player::is_active()
{
	return active;
}

void Player::set_id(const size_t &id_player)
{
	id = id_player;
}

int Player::get_id() const
{
	return id;
}

Vector2f Player::advance(Vector2i &direction)
{
	return Vector2f(active_command.pos_now.x + direction.x * SPEED, active_command.pos_now.y + direction.y * SPEED);
}

bool Player::getInputs(const float &widthLimit, const float &heightLimit)
{
	bool input = false;
	Vector2i mouvement;
	// Up
	if (sf::Keyboard::isKeyPressed(Keyboard::W) || sf::Keyboard::isKeyPressed(Keyboard::Up))
	{
		mouvement.y += -1;
		input = true;
		
	}
	// Left
	if (sf::Keyboard::isKeyPressed(Keyboard::A) || sf::Keyboard::isKeyPressed(Keyboard::Left))
	{
		mouvement.x += -1;
		input = true;
	}
	// Right
	if (sf::Keyboard::isKeyPressed(Keyboard::D) || sf::Keyboard::isKeyPressed(Keyboard::Right))
	{
		mouvement.x += 1;
		input = true;
	}
	// Down
	if (sf::Keyboard::isKeyPressed(Keyboard::S) || sf::Keyboard::isKeyPressed(Keyboard::Down))
	{
		mouvement.y += 1;
		input = true;
	}
	active_command.position_before_change = active_command.pos_now;
	Vector2f newPos = advance(active_command.direction);
	if (checkOutOfBound(widthLimit, heightLimit, newPos))
	{
		active_command.pos_now = newPos;
		spritePlayer.setPosition(active_command.pos_now);
	}

	active_command.direction = mouvement;
	
	return input;
}

bool Player::checkOutOfBound(const float &widthLimit, const float &heightLimit, Vector2f &newPos)
{
	//std::cout <<spritePlayer.getGlobalBounds().width/6 << " : " <<spritePlayer.getGlobalBounds().height/6 << std::endl;
	if (newPos.x - spritePlayer.getGlobalBounds().width / 6 > 0 && newPos.x + spritePlayer.getGlobalBounds().width < widthLimit
		&& newPos.y - spritePlayer.getGlobalBounds().height / 6 > 0 && newPos.y + spritePlayer.getGlobalBounds().height < heightLimit)
	{
		return true;
	}
	return false;
}
char* Player::format_data_to_send(const int player_id, const Vector2f current_position, const Vector2i direction)
{
	char data[2000] ={NULL};
	int iterator = 0;
	stringstream stream;
	string data_to_char = "";
	
	//add id
	stream << player_id;
	data_to_char = stream.str();
	for (int i=0;i<data_to_char.length();i++)
	{
		data[iterator] = data_to_char[i];
		iterator++;
	}
	data[iterator] = '|';
	iterator++;
	//add position
	for (int i=0;i<2;i++)
	{
		stream.str("");
		if (i==0)
		{
			stream << current_position.x;
		}
		else
		{
			stream << current_position.y;
		}
		data_to_char = stream.str();
		for (int j=0;j<data_to_char.length();j++)
		{
			data[iterator] = data_to_char[j];
			iterator++;
		}
		data[iterator] = '|';
		iterator++;
	}
	stream.str("");
	//add direction
	for (int i = 0; i<2; i++)
	{
		stream.str("");
		if (i == 0)
		{
			stream << direction.x;
		}
		else
		{
			stream << direction.y;
		}
		data_to_char = stream.str();
		for (int j = 0; j<data_to_char.length(); j++)
		{
			data[iterator] = data_to_char[j];
			iterator++;
		}
		data[iterator] = '|';
		iterator++;
	}
	data[iterator] = '&';
	data_player = data;
	/*std::cout << data;*/
	return data;
}
