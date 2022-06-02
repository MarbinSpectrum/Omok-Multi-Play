#include "MsgMgr.h"
#include "../ClientMgr/ClientMgr.h"

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

////////////////////////////////////////////////////////////////////////////////
/// : 메세지를 보내는 부분
////////////////////////////////////////////////////////////////////////////////
void MsgMgr::SendMsg(Message message, SOCKET socket)
{
	std::string msg = message.ConvertString();
	send(socket, msg.c_str(), strlen(msg.c_str()), 0);
}

////////////////////////////////////////////////////////////////////////////////
/// : 메세지를 받는 부분
////////////////////////////////////////////////////////////////////////////////
void MsgMgr::OnReceiveMsg(Message message, SOCKET socket)
{
	MessageType messageType = message.GetMessageType();

	switch (messageType)
	{
		case MessageType::CONNECT_CLIENT:
		{
			//클라이언트와 서버가 연결됨
			//소캣번호
			std::cout << "CONNECT_CLIENT" << " ";
			std::cout << "SOCKET : ";
			std::cout << message.ReadMessage() << "\n";
		}
		break;
		case MessageType::DISCONNECT_CLIENT:
		{
			//클라이언트와 서버가 연결이 해제됨
			//소캣번호
			std::cout << "DISCONNECT_CLIENT" << " ";
			std::cout << "SOCKET : ";
			std::cout << message.ReadMessage() << "\n";

			CLIENT_MGR.RemoveClient(socket);
		}
		break;
		case MessageType::LOBBY_ENTER_REQUEST:
		{
			//클라이언트의 로비 입장 요청
			//아이디
			std::string playerName = message.ReadMessage();

			std::cout << "LOBBY_ENTER_REQUEST" << " ";
			std::cout << "아이디 : ";
			std::cout << playerName << "\n";

			std::cout << "LOBBY_ENTER_REPLY" << " ";
			Message msg(MessageType::LOBBY_ENTER_REPLY);

			if (ClientMgr::PlayerNameCheck(playerName) == false)
			{
				//아이디가 이상함
				msg.WriteMessage(false);
				std::cout << 0 << "\n";
			}
			else if (CLIENT_MGR.RegistClient(socket, playerName))
			{
				//등록이 성공적으로 완료됨
				msg.WriteMessage(true);
				msg.WriteMessage(playerName);
				std::cout << 1 << "\n";
			}
			else
			{
				//등록이 안됨
				msg.WriteMessage(false);
				std::cout << 0 << "\n";
			}
			SendMsg(msg, socket);
		}
		break;
		case MessageType::LOBBY_ROOM_DATA_REQUEST:
		{
			//로비의 방 데이터를 요청
			//아이디
			std::cout << "LOBBY_ROOM_DATA_REQUEST" << " ";
			std::cout << "아이디 : ";
			std::cout << message.ReadMessage() << "\n";

			Message msg(MessageType::LOBBY_ROOM_DATA_REPLY);
			GAMEROOM_MGR.WriteRoomDatas(msg);
			SendMsg(msg, socket);

			std::cout << "LOBBY_ROOM_DATA_REPLY" << "\n";
		}
		break;
		case MessageType::MAKE_ROOM_REQUEST:
		{
			//방을 만드는 것을 요청
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
			//방에 입장하는 것을 요청
			//방 번호, 키
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
		case MessageType::EXIT_ROOM_REQUEST:
		{
			//방에 입장하는 것을 요청
			//방 번호, 키
			std::cout << "EXIT_ROOM_REQUEST" << "\n";

			bool success = GAMEROOM_MGR.ExitRoom(socket);

			Message msg(MessageType::EXIT_ROOM_REPLY);
			msg.WriteMessage(success);
			SendMsg(msg, socket);

			std::cout << "EXIT_ROOM_REPLY" << " ";
			std::cout << success << "\n";
		}
		break;
		case MessageType::GAMEROOM_DATA_REQUEST:
		{
			//방에 입장하는 것을 요청
			//방 번호, 키
			std::cout << "GAMEROOM_DATA_REQUEST" << "\n";

			std::cout << "GAMEROOM_DATA_REPLY" << " ";
			Message msg(MessageType::GAMEROOM_DATA_REPLY);
			bool success = false;

			ClientObj* clientObj = CLIENT_MGR.GetClient(socket);
			if (clientObj == NULL)
			{
				//클라이언트가 존재하지 않는다.
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
					//방이 존재하지 않는다.
					success = false;
					msg.WriteMessage(success);
				}
				else
				{
					success = true;
					msg.WriteMessage(success);
					gameRoom->WriteRoomData(msg);
				}
			}

			std::cout << success << "\n";
			SendMsg(msg, socket);
		}
		break;
	}
}
