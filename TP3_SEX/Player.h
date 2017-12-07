#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network/TcpSocket.hpp>


using namespace sf;

class Player
{
public:
	Player();
	~Player();

	void update(const float &widthLimit, const float &heightLimit);
	void draw(RenderWindow &window);

	Vector2f getPosition() const;
	void set_drawing_position();
	void setPosX(const float &newPosX);
	void setPosY(const float &newPosY);
	void init(const Vector2f position, Texture &texture);
	void set_active(bool stat);
	bool is_active();
	void set_id(const size_t &id_player);
	int get_id()const;
	struct player_command
	{
		Vector2f pos_now ;
		Vector2f position_before_change;
		Vector2i direction;

	};
	player_command active_command;
	char* data_player;
	char* format_data_to_send(const int player_id, const Vector2f current_position, const Vector2i direction);
	bool input = false;
private:
	const float SPEED = 5;
	Vector2f advance(Vector2i &direction);
	bool getInputs(const float &widthLimit, const float &heightLimit);
	bool checkOutOfBound(const float &widthLimit, const float &heightLimit, Vector2f &newPos);
	bool active = false;
	Sprite spritePlayer;
	int id;

	enum actionTypes { MOVE, ATTACK };
};

