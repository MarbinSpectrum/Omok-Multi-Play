#include "MsgMgr.h"
#include "../ClientMgr/ClientMgr.h"

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
			//��Ĺ��ȣ
			std::cout << "CONNECT_CLIENT SOCKET : " << message.ReadMessage() << "\n";
		}
		break;
		case MessageType::DISCONNECT_CLIENT:
		{
			//Ŭ���̾�Ʈ�� ������ ������ ������
			//��Ĺ��ȣ
			std::cout << "DISCONNECT_CLIENT SOCKET : " << message.ReadMessage() << "\n";

			CLIENT_MGR.RemoveClient(socket);
		}
		break;
		case MessageType::LOBBY_ENTER_REQUEST:
		{
			//Ŭ���̾�Ʈ�� �κ� ���� ��û
			//���̵�
			std::string playerName = message.ReadMessage();

			std::cout << "LOBBY_ENTER_REQUEST" << " ";
			std::cout << "���̵� : ";
			std::cout << playerName << "\n";

			bool success = false;
			Message msg(MessageType::LOBBY_ENTER_REPLY);

			if (ClientMgr::PlayerNameCheck(playerName) == false)
			{
				//���̵� �̻���
				msg.WriteMessage(false);
				success = false;
			}
			else if (CLIENT_MGR.RegistClient(socket, playerName))
			{
				//����� ���������� �Ϸ��
				msg.WriteMessage(true);
				msg.WriteMessage(playerName);
				success = true;
			}
			else
			{
				//����� �ȵ�
				msg.WriteMessage(false);
				success = false;
			}
			std::cout << "LOBBY_ENTER_REPLY" << " " << success << endl;
			SendMsg(msg, socket);
		}
		break;
		case MessageType::LOBBY_ROOM_DATA_REQUEST:
		{
			//�κ��� �� �����͸� ��û
			//���̵�
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

			std::cout << "MAKE_ROOM_REPLY" << " " << success << "\n";
		}
		break;
		case MessageType::ENTER_ROOM_REQUEST:
		{
			//�濡 �����ϴ� ���� ��û
			//�� ��ȣ, Ű
			std::cout << "ENTER_ROOM_REQUEST" << "\n";
			uint roomNum = StringToUint(message.ReadMessage());
			int64 roomKey = StringToInt64(message.ReadMessage());

			bool success = GAMEROOM_MGR.EnterRoom(socket, roomNum, roomKey);
			Message msg(MessageType::ENTER_ROOM_REPLY);
			msg.WriteMessage(success);
			SendMsg(msg, socket);

			std::cout << "ENTER_ROOM_REPLY" << " " << success << "\n";
		}
		break;
		case MessageType::EXIT_ROOM_REQUEST:
		{
			//�濡 �����ϴ� ���� ��û
			std::cout << "EXIT_ROOM_REQUEST" << "\n";

			bool success = GAMEROOM_MGR.ExitRoom(socket);

			Message msg(MessageType::EXIT_ROOM_REPLY);
			msg.WriteMessage(success);
			SendMsg(msg, socket);

			std::cout << "EXIT_ROOM_REPLY" << " " << success << "\n";
		}
		break;
		case MessageType::GAMEROOM_DATA_REQUEST:
		{
			//�� ���� ������ ��û
			std::cout << "GAMEROOM_DATA_REQUEST" << "\n";

			Message msg(MessageType::GAMEROOM_DATA_REPLY);
			bool success = false;

			ClientObj* clientObj = CLIENT_MGR.GetClient(socket);
			if (clientObj == NULL)
			{
				//Ŭ���̾�Ʈ�� �������� �ʴ´�.
				success = false;
				msg.WriteMessage(success);
			}
			else
			{
				uint roomNum = clientObj->playerPos.roomNum;
				int64 roomKey = clientObj->playerPos.roomKey;

				GameRoom* gameRoom = GAMEROOM_MGR.GetGameRoom(roomNum, roomKey);
				if (gameRoom == NULL)
				{
					//���� �������� �ʴ´�.
					success = false;
					msg.WriteMessage(success);
				}
				else
				{
					success = true;
					msg.WriteMessage(success);
					gameRoom->WriteRoomData(msg, socket);
				}
			}

			std::cout << "GAMEROOM_DATA_REPLY" << " " << success << "\n";
			SendMsg(msg, socket);
		}
		break;
		case MessageType::GAMEROOM_READY_UPDATE:
		{
			//���� �غ���� ���� ��û
			//�غ����(bool)
			std::cout << "GAMEROOM_READY_UPDATE" << "\n";
			ClientObj* clientObj = CLIENT_MGR.GetClient(socket);
			bool state = stoi(message.ReadMessage());

			uint roomNum = clientObj->playerPos.roomNum;
			int64 roomKey = clientObj->playerPos.roomKey;

			GameRoom* gameRoom = GAMEROOM_MGR.GetGameRoom(roomNum, roomKey);
			if (gameRoom != NULL)
			{
				gameRoom->SetIsPlayerReady(clientObj, state);
			}
		}
		break;
		case MessageType::GAMEROOM_GAME_START_REQUEST:
		{
			//���� ���� ��û
			std::cout << "GAMEROOM_GAME_START_REQUEST" << "\n";
			ClientObj* clientObj = CLIENT_MGR.GetClient(socket);

			uint roomNum = clientObj->playerPos.roomNum;
			int64 roomKey = clientObj->playerPos.roomKey;

			std::cout << "GAMEROOM_GAME_START_REPLY" << "\n";
			GameRoom* gameRoom = GAMEROOM_MGR.GetGameRoom(roomNum, roomKey);
			if (gameRoom != NULL)
			{
				gameRoom->GameStart();
			}
			else
			{
				Message message(MessageType::GAMEROOM_GAME_START_REPLY);
				message.WriteMessage(0);

				MSG_MGR.SendMsg(message, socket);
			}
		}
		break;
		case MessageType::GAMEBOARD_DATA_REQUEST:
		{
			//������ ���¸� ��û
			//�÷��̾��� ��
			//�ǽ� ����
			//�ǽ� R
			//�ǽ� C
			//�ǽ� ����
			std::cout << "GAMEBOARD_DATA_REQUEST" << "\n";
			ClientObj* clientObj = CLIENT_MGR.GetClient(socket);

			Message msg(MessageType::GAMEBOARD_DATA_REPLY);
			bool success = false;

			if (clientObj == NULL)
			{
				success = false;
				msg.WriteMessage(success);
			}
			else
			{
				uint roomNum = clientObj->playerPos.roomNum;
				int64 roomKey = clientObj->playerPos.roomKey;

				GameRoom* gameRoom = GAMEROOM_MGR.GetGameRoom(roomNum, roomKey);
				if (gameRoom != NULL)
				{
					success = true;
					msg.WriteMessage(success);

					GameMgr* gameMgr = gameRoom->GetGameMgr();
					gameMgr->WriteNowBoard(msg, clientObj);
				}
				else
				{
					success = false;
					msg.WriteMessage(success);
				}
			}

			std::cout << "GAMEBOARD_DATA_REPLY" << " " << success << "\n";
			SendMsg(msg, socket);
		}
		break;
	}
}
