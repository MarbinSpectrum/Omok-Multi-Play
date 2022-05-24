#include "MsgMgr.h"

MsgMgr* MsgMgr::instance = NULL;
MsgMgr& MsgMgr::Instance()
{
	if (instance == NULL)
	{
		//싱글톤 객체를 추가
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
			std::cout << "CONNECT_CLIENT" << " ";
			std::cout << "SOCKET : ";
			std::cout << message.ReadMessage() << "\n";
		}
		break;
		case MessageType::LOBBY_ENTER_REQUEST:
		{
			std::string playerName = message.ReadMessage();

			std::cout << "LOBBY_ENTER_REQUEST" << " ";
			std::cout << "아이디 : ";
			std::cout << playerName << "\n";

			Message message(MessageType::LOBBY_ENTER_REPLY);

			if (CLIENT_MGR.RegistClient(socket, playerName))
			{
				//등록이 성공적으로 완료됨
				message.WriteMessage(true);
				message.WriteMessage(playerName);
			}
			else
			{
				//등록이 안됨
				message.WriteMessage(false);
			}
			SendMsg(message, socket);

		}
		break;
		case MessageType::LOBBY_ROOM_DATA_REQUEST:
		{
			std::cout << "LOBBY_ROOM_DATA_REQUEST" << " ";
			std::cout << "아이디 : ";
			std::cout << message.ReadMessage() << "\n";

			Message message(MessageType::LOBBY_ROOM_DATA_REPLY);
			GAMEROOM_MGR.WriteRoomDatas(message);
			SendMsg(message, socket);
		}
		break;
		case MessageType::MAKE_ROOM_REQUEST:
		{
			std::cout << "MAKE_ROOM_REQUEST" << "\n";
			bool success = GAMEROOM_MGR.CreateRoom();

			Message message(MessageType::MAKE_ROOM_REPLY);
			if (success)
			{
				message.WriteMessage(true);
			}
			else
			{
				message.WriteMessage(false);
			}
			SendMsg(message, socket);
		}
		break;
	}
}
