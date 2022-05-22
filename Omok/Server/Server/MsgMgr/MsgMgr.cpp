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

void MsgMgr::SendMsg(Message message)
{

}

void MsgMgr::OnReceiveMsg(Message message)
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
		}
		break;
	}
}
