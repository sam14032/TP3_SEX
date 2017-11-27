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

void Player::update(const float &widthLimit, const float &heightLimit)
{
	getInputs(widthLimit, heightLimit);
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

Vector2f Player::advance(Vector2f &direction)
{
	return Vector2f(position.x + direction.x * SPEED, position.y + direction.y * SPEED);
}

void Player::getInputs(const float &widthLimit, const float &heightLimit)
{
	// Up
	if (sf::Keyboard::isKeyPressed(Keyboard::W) || sf::Keyboard::isKeyPressed(Keyboard::Up))
	{
		Vector2f newPos = advance(Vector2f(0, -1));
		if (checkOutOfBound(widthLimit, heightLimit, newPos))
		{
			position = newPos;
			spritePlayer.setPosition(position);
		}
	}
	// Left
	if (sf::Keyboard::isKeyPressed(Keyboard::A) || sf::Keyboard::isKeyPressed(Keyboard::Left))
	{
		Vector2f newPos = advance(Vector2f(-1, 0));
		if (checkOutOfBound(widthLimit, heightLimit, newPos))
		{
			position = newPos;
			spritePlayer.setPosition(position);
		}
	}
	// Right
	if (sf::Keyboard::isKeyPressed(Keyboard::D) || sf::Keyboard::isKeyPressed(Keyboard::Right))
	{
		Vector2f newPos = advance(Vector2f(1, 0));
		if (checkOutOfBound(widthLimit, heightLimit, newPos))
		{
			position = newPos;
			spritePlayer.setPosition(position);
		}
	}
	// Down
	if (sf::Keyboard::isKeyPressed(Keyboard::S) || sf::Keyboard::isKeyPressed(Keyboard::Down))
	{
		Vector2f newPos = advance(Vector2f(0, 1));
		if (checkOutOfBound(widthLimit, heightLimit, newPos))
		{
			position = newPos;
			spritePlayer.setPosition(position);
		}
	}
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