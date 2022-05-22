#include "Message.h"
#include "../Algorithm/Algorithm.h"

Message::Message(MessageType messageType)
: messageType(messageType)
{
}
Message::Message(std::string str)
{
	vector<std::string> stringSplit = split(str, { ',' });
	for (int idx = 0; idx < stringSplit.size(); idx++)
	{
		if (idx == 0)
		{
			int messageInt = stoi(stringSplit[idx]);
			messageType = (MessageType)messageInt;
		}
		else
		{
			WriteMessage(stringSplit[idx]);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
/// : 메세지를 작성하는 부분
////////////////////////////////////////////////////////////////////////////////
void Message::WriteMessage(std::string str)
{
	messageQueue.push(str);
}
void Message::WriteMessage(int value)
{
	WriteMessage(std::to_string(value));
}

////////////////////////////////////////////////////////////////////////////////
/// : 메세지 타입을 읽는다.
////////////////////////////////////////////////////////////////////////////////
MessageType Message::GetMessageType()
{
	return messageType;
}

////////////////////////////////////////////////////////////////////////////////
/// : 메세지를 읽는 부분
////////////////////////////////////////////////////////////////////////////////
std::string Message::ReadMessage()
{
	if (messageQueue.empty())
	{
		return "";
	}
	std::string message = messageQueue.front();
	messageQueue.pop();

	return message;
}

////////////////////////////////////////////////////////////////////////////////
/// : 해당 객체를 문자열로 치환
////////////////////////////////////////////////////////////////////////////////
std::string Message::ConvertString()
{
	std::string temp = "";
	
	temp += std::to_string((int)messageType) + ",";
	MessageQueue tempQueue;

	while(!messageQueue.empty())
	{
		//큐를 한번씩 순회
		std::string message = messageQueue.front();
		messageQueue.pop();
		tempQueue.push(message);

		//해당 메세지를 작성
		temp += message + ",";
	}

	while (!tempQueue.empty())
	{
		//큐를 다시 채워준다.
		std::string message = tempQueue.front();
		tempQueue.pop();
		messageQueue.push(message);
	}

	return temp;
}
