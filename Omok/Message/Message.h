#pragma once

#include <queue>
#include <string>

enum class MessageType
{
	CONNECT_CLIENT = 99,
	LOBBY_ENTER_REQUEST = 100,
	LOBBY_ENTER_REPLY	= 101,
	LOBBY_ROOM_DATA_REQUEST	= 110,
	LOBBY_ROOM_DATA_REPLY	= 111,
};

class Message
{
	typedef std::queue<std::string> MessageQueue;

public:
	Message(MessageType messageType);
	Message(std::string str);

public:
	void			WriteMessage(std::string str);
	void			WriteMessage(int value);
	MessageType		GetMessageType();
	std::string		ReadMessage();
	std::string		ConvertString();

private:
	MessageType		messageType;
	MessageQueue	messageQueue;
};