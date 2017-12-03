#include "Player.h"


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
		char* data = "01|#1|45|12|1|1|?1|@0045|&";
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

Vector2f Player::advance(Vector2f &direction)
{
	return Vector2f(position.x + direction.x * SPEED, position.y + direction.y * SPEED);
}

bool Player::getInputs(const float &widthLimit, const float &heightLimit)
{
	bool input = false;
	Vector2f direction;
	// Up
	if (sf::Keyboard::isKeyPressed(Keyboard::W) || sf::Keyboard::isKeyPressed(Keyboard::Up))
	{
		direction.y += -1;
		input = true;
		
	}
	// Left
	if (sf::Keyboard::isKeyPressed(Keyboard::A) || sf::Keyboard::isKeyPressed(Keyboard::Left))
	{
		direction.x += -1;
		input = true;
	}
	// Right
	if (sf::Keyboard::isKeyPressed(Keyboard::D) || sf::Keyboard::isKeyPressed(Keyboard::Right))
	{
		direction.x += 1;
		input = true;
	}
	// Down
	if (sf::Keyboard::isKeyPressed(Keyboard::S) || sf::Keyboard::isKeyPressed(Keyboard::Down))
	{
		direction.y += 1;
		input = true;
	}
	Vector2f old_pos = position;
	Vector2f newPos = advance(direction);
	if (checkOutOfBound(widthLimit, heightLimit, newPos))
	{
		position = newPos;
		spritePlayer.setPosition(position);
	}

	active_command.direction = direction;
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