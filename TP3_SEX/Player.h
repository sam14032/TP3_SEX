#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Player
{
public:
	Player();
	Player(const Vector2f position, Texture &texture);
	~Player();

	void update(const float &widthLimit, const float &heightLimit);
	void draw(RenderWindow &window);

	Vector2f getPosition() const;
	void setPosX(const float &newPosX);
	void setPosY(const float &newPosY);

private:
	const float SPEED = 5;

	Vector2f advance(Vector2f &direction);
	void getInputs(const float &widthLimit, const float &heightLimit);
	bool checkOutOfBound(const float &widthLimit, const float &heightLimit, Vector2f &newPos);

	Sprite spritePlayer;

	Vector2f position;
};

