#include "Player.h"
#include <string>;
#include <iomanip>
#include <sstream>;
using namespace std;
Player::Player()
{
	position = Vector2f(0, 0);
}

Player::Player(const Vector2f position, Texture &texture)
{
	this->position = position;

	spritePlayer.setTexture(texture);
	spritePlayer.setScale(0.2f, 0.2f);
	spritePlayer.setOrigin(spritePlayer.getGlobalBounds().width / 2, spritePlayer.getGlobalBounds().height / 2);
	spritePlayer.setPosition(position);
}

Player::~Player()
{

}

void Player::update(const float &widthLimit, const float &heightLimit, TcpSocket &server)
{
	server.setBlocking(false);
	bool input = getInputs(widthLimit, heightLimit);
	if (input)
	{
		char* data = dataCryped(active_command.id,active_command.player_action,active_command.pos_now,active_command.direction,0,0,0);
		server.send(data, 2000);
	}
	
}

void Player::draw(RenderWindow &window)
{
	window.draw(spritePlayer);
}

Vector2f Player::getPosition() const
{
	return position;
}

void Player::setPosX(const float &newPosX)
{
	position.x = newPosX;
}

void Player::setPosY(const float &newPosY)
{
	position.y = newPosY;
}

void Player::init(const Vector2f position, Texture & texture)
{
	this->position = position;

	spritePlayer.setTexture(texture);
	spritePlayer.setScale(0.2f, 0.2f);
	spritePlayer.setOrigin(spritePlayer.getGlobalBounds().width / 2, spritePlayer.getGlobalBounds().height / 2);
	spritePlayer.setPosition(position);
}

void Player::set_active(bool stat)
{
	active = stat;
}

bool Player::is_active()
{
	return active;
}

Vector2f Player::advance(Vector2i &direction)
{
	return Vector2f(position.x + direction.x * SPEED, position.y + direction.y * SPEED);
}

bool Player::getInputs(const float &widthLimit, const float &heightLimit)
{
	bool input = false;
	Vector2i mouvement;
	// Up
	if (sf::Keyboard::isKeyPressed(Keyboard::W) || sf::Keyboard::isKeyPressed(Keyboard::Up))
	{
		mouvement.y += -1;
		active_command.player_action=(int)actionTypes::MOVE;
		input = true;
		
	}
	// Left
	if (sf::Keyboard::isKeyPressed(Keyboard::A) || sf::Keyboard::isKeyPressed(Keyboard::Left))
	{
		mouvement.x += -1;
		active_command.player_action = (int)actionTypes::MOVE;
		input = true;
	}
	// Right
	if (sf::Keyboard::isKeyPressed(Keyboard::D) || sf::Keyboard::isKeyPressed(Keyboard::Right))
	{
		mouvement.x += 1;
		active_command.player_action = (int)actionTypes::MOVE;
		input = true;
	}
	// Down
	if (sf::Keyboard::isKeyPressed(Keyboard::S) || sf::Keyboard::isKeyPressed(Keyboard::Down))
	{
		mouvement.y += 1;
		active_command.player_action = (int)actionTypes::MOVE;
		input = true;
	}
	Vector2f old_pos = position;
	Vector2f newPos = advance(active_command.direction);
	if (checkOutOfBound(widthLimit, heightLimit, newPos))
	{
		position = newPos;
		spritePlayer.setPosition(position);
	}

	active_command.direction = mouvement;
	active_command.pos_now = position;
	active_command.angle = 0;
	active_command.attack_type = 0;
	active_command.target = 999;
	active_command.player_action = 0;
	
	return input;
}

bool Player::checkOutOfBound(const float &widthLimit, const float &heightLimit, Vector2f &newPos)
{
	if (newPos.x - spritePlayer.getGlobalBounds().width / 6 > 0 && newPos.x + spritePlayer.getGlobalBounds().width < widthLimit
		&& newPos.y - spritePlayer.getGlobalBounds().height / 6 > 0 && newPos.y + spritePlayer.getGlobalBounds().height < heightLimit)
	{
		return true;
	}

	return false;
}
char* Player::dataCryped(const int playerId,const int actionType, const Vector2f currentPosition, const Vector2i direction,const int attack,const int target,const float angle)
{

	string dataStr;

	string playerIdStr = to_string(playerId);
	string actionTypeStr=to_string(actionType);
	ostringstream currentPositionXStr;
	currentPositionXStr << setprecision(5) << currentPosition.x;
	ostringstream currentPositionYstr;
	currentPositionYstr<<setprecision(5)<<currentPosition.y;
	string directionMoveXStr=to_string(direction.x);
	string directionMoveYStr=to_string(direction.y);
	string attackStr=to_string(attack);
	string targetStr=to_string(target);
	ostringstream angleStr;
	angleStr<<setprecision(3)<<angle;
	

	dataStr.append(playerIdStr);
	dataStr.append("|");
	dataStr.append("#");
	dataStr.append(actionTypeStr);
	dataStr.append("|");
	dataStr.append(currentPositionXStr.str());
	dataStr.append("|");
	dataStr.append(currentPositionYstr.str());
	dataStr.append("|");
	dataStr.append(directionMoveXStr);
	dataStr.append("|");
	dataStr.append(directionMoveYStr);
	dataStr.append("|");
	dataStr.append("?");
	dataStr.append(attackStr);
	dataStr.append("|");
	dataStr.append("@");
	dataStr.append(targetStr);
	dataStr.append("|");
	dataStr.append(angleStr.str());
	dataStr.append("|");
	dataStr.append("&");

	char *data= new char[dataStr.length() + 1];
	strcpy(data, dataStr.c_str());
	return data;
}