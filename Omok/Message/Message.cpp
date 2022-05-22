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
/// : �޼����� �ۼ��ϴ� �κ�
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
/// : �޼��� Ÿ���� �д´�.
////////////////////////////////////////////////////////////////////////////////
MessageType Message::GetMessageType()
{
	return messageType;
}

////////////////////////////////////////////////////////////////////////////////
/// : �޼����� �д� �κ�
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
/// : �ش� ��ü�� ���ڿ��� ġȯ
////////////////////////////////////////////////////////////////////////////////
std::string Message::ConvertString()
{
	std::string temp = "";
	
	temp += std::to_string((int)messageType) + ",";
	MessageQueue tempQueue;

	while(!messageQueue.empty())
	{
		//ť�� �ѹ��� ��ȸ
		std::string message = messageQueue.front();
		messageQueue.pop();
		tempQueue.push(message);

		//�ش� �޼����� �ۼ�
		temp += message + ",";
	}

	while (!tempQueue.empty())
	{
		//ť�� �ٽ� ä���ش�.
		std::string message = tempQueue.front();
		tempQueue.pop();
		messageQueue.push(message);
	}

	return temp;
}
