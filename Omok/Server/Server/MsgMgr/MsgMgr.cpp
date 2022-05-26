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

////////////////////////////////////////////////////////////////////////////////
/// : �޼����� ������ �κ�
////////////////////////////////////////////////////////////////////////////////
void MsgMgr::SendMsg(Message message, SOCKET socket)
{
	std::string msg = message.ConvertString();
	send(socket, msg.c_str(), strlen(msg.c_str()), 0);
}

////////////////////////////////////////////////////////////////////////////////
/// : �޼����� �޴� �κ�
////////////////////////////////////////////////////////////////////////////////
void MsgMgr::OnReceiveMsg(Message message, SOCKET socket)
{
	MessageType messageType = message.GetMessageType();

	switch (messageType)
	{
		case MessageType::CONNECT_CLIENT:
		{
			//Ŭ���̾�Ʈ�� ������ �����
			std::cout << "CONNECT_CLIENT" << " ";
			std::cout << "SOCKET : ";
			std::cout << message.ReadMessage() << "\n";
		}
		break;
		case MessageType::LOBBY_ENTER_REQUEST:
		{
			//Ŭ���̾�Ʈ�� �κ� ���� ��û
			std::string playerName = message.ReadMessage();

			std::cout << "LOBBY_ENTER_REQUEST" << " ";
			std::cout << "���̵� : ";
			std::cout << playerName << "\n";

			Message msg(MessageType::LOBBY_ENTER_REPLY);

			std::cout << "LOBBY_ENTER_REPLY" << " ";
			if (CLIENT_MGR.RegistClient(socket, playerName))
			{
				//����� ���������� �Ϸ��
				msg.WriteMessage(true);
				msg.WriteMessage(playerName);
				std::cout << 1 << "\n";
			}
			else
			{
				//����� �ȵ�
				msg.WriteMessage(false);
				std::cout << 0 << "\n";
			}
			SendMsg(msg, socket);
		}
		break;
		case MessageType::LOBBY_ROOM_DATA_REQUEST:
		{
			//�κ��� �� �����͸� ��û
			std::cout << "LOBBY_ROOM_DATA_REQUEST" << " ";
			std::cout << "���̵� : ";
			std::cout << message.ReadMessage() << "\n";

			Message msg(MessageType::LOBBY_ROOM_DATA_REPLY);
			GAMEROOM_MGR.WriteRoomDatas(msg);
			SendMsg(msg, socket);

			std::cout << "LOBBY_ROOM_DATA_REPLY" << "\n";
		}
		break;
		case MessageType::MAKE_ROOM_REQUEST:
		{
			//���� ����� ���� ��û
			std::cout << "MAKE_ROOM_REQUEST" << "\n";
			bool success = GAMEROOM_MGR.CreateRoom(socket);

			Message msg(MessageType::MAKE_ROOM_REPLY);
			msg.WriteMessage(success);
			SendMsg(msg, socket);

			std::cout << "MAKE_ROOM_REPLY" << " ";
			std::cout << success << "\n";
		}
		break;
		case MessageType::ENTER_ROOM_REQUEST:
		{
			//�濡 �����ϴ� ���� ��û
			std::cout << "ENTER_ROOM_REQUEST" << "\n";
			uint roomNum = StringToUint(message.ReadMessage());
			int64 roomKey = StringToInt64(message.ReadMessage());

			bool success = GAMEROOM_MGR.EnterRoom(socket, roomNum, roomKey);
			Message msg(MessageType::ENTER_ROOM_REPLY);
			msg.WriteMessage(success);
			SendMsg(msg, socket);

			std::cout << "ENTER_ROOM_REPLY" << " ";
			std::cout << success << "\n";
		}
		break;
	}
}
