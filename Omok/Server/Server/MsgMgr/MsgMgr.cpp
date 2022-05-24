#include "MsgMgr.h"

MsgMgr* MsgMgr::instance = NULL;
MsgMgr& MsgMgr::Instance()
{
	if (instance == NULL)
	{
		//�̱��� ��ü�� �߰�
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
			std::cout << "���̵� : ";
			std::cout << playerName << "\n";

			Message message(MessageType::LOBBY_ENTER_REPLY);

			if (CLIENT_MGR.RegistClient(socket, playerName))
			{
				//����� ���������� �Ϸ��
				message.WriteMessage(true);
				message.WriteMessage(playerName);
			}
			else
			{
				//����� �ȵ�
				message.WriteMessage(false);
			}
			SendMsg(message, socket);

		}
		break;
		case MessageType::LOBBY_ROOM_DATA_REQUEST:
		{
			std::cout << "LOBBY_ROOM_DATA_REQUEST" << " ";
			std::cout << "���̵� : ";
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
