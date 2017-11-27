#pragma once
#include <SFML/Network.hpp>
using namespace sf;
class connection
{
public:
	TcpSocket socket;
	bool init_socket();
	bool listen_server();
	bool send_server();
};
inline bool connection::init_socket()
{
	Socket::Status status = socket.connect("192.168.79.129", 8888);
	if (status != Socket::Done)
	{
		return false;
	}
	socket.setBlocking(false);
	return true;
}

inline bool connection::listen_server()
{
	char data[2000];
	std::size_t received;
	if (socket.receive(data, 2000,received) != Socket::Done)
	{
		return false;
	}
	return true;
}

inline bool connection::send_server()
{
	char data[2000];
	if (socket.send(data,2000)!= Socket::Done)
	{
		return false;
	}
	return true;
}



