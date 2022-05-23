#include "MsgMgr.h"

MsgMgr* MsgMgr::instance = NULL;
MsgMgr& MsgMgr::Instance()
{
	if (instance == NULL)
	{
		//ΩÃ±€≈Ê ∞¥√º∏¶ √ﬂ∞°
		instance = new MsgMgr();
	}
	return *instance;
}

void MsgMgr::SendMsg(Message message, SOCKET socket)
{
	std::string msg = message.ConvertString();
	send(socket, msg.c_str(), strlen(msg.c_str()), 0);
}

void MsgMgr::OnReceiveMsg(Message message, SOCKET socket)
{
	MessageType messageType = message.GetMessageType();

	switch (messageType)
	{
		case MessageType::CONNECT_CLIENT:
		{
			std::cout << message.ReadMessage() << " ";
			std::cout << message.ReadMessage() << "\n";
		}
		break;
		case MessageType::LOBBY_ENTER_REQUEST:
		{
			std::cout << message.ReadMessage() << "\n";

			Message message(MessageType::LOBBY_ENTER_REPLY);

			SendMsg(message, socket);
		}
		break;
	}
}
