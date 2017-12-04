#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network/TcpSocket.hpp>


using namespace sf;

class Player
{
public:
	Player();
	Player(const Vector2f position, Texture &texture);
	~Player();

	void update(const float &widthLimit, const float &heightLimit, TcpSocket &server);
	void draw(RenderWindow &window);

	Vector2f getPosition() const;
	void setPosX(const float &newPosX);
	void setPosY(const float &newPosY);
	void init(const Vector2f position, Texture &texture);
	void set_active(bool stat);
	bool is_active();
	struct player_command
	{
		int id;
		double time_stamp = 0;
		Vector2f pos_now;
		Vector2i direction;
		int player_action;
		int attack_type;
		size_t target;
		float angle;

	};
	player_command active_command;
private:
	const float SPEED = 5;
	Vector2f advance(Vector2i &direction);
	bool getInputs(const float &widthLimit, const float &heightLimit);
	bool checkOutOfBound(const float &widthLimit, const float &heightLimit, Vector2f &newPos);
	bool active = false;
	Sprite spritePlayer;
	Vector2f position;
	char* dataCryped(const int playerId, const int actionType, const Vector2f currentPosition, const Vector2i direction, const int attack, const int target, const float angle);

	enum actionTypes { MOVE, ATTACK };
};

