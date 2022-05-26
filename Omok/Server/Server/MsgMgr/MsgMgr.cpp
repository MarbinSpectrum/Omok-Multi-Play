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
			std::cout << "CONNECT_CLIENT" << " ";
			std::cout << "SOCKET : ";
			std::cout << message.ReadMessage() << "\n";
		}
		break;
		case MessageType::LOBBY_ENTER_REQUEST:
		{
			//클라이언트의 로비 입장 요청
			std::string playerName = message.ReadMessage();

			std::cout << "LOBBY_ENTER_REQUEST" << " ";
			std::cout << "아이디 : ";
			std::cout << playerName << "\n";

			Message msg(MessageType::LOBBY_ENTER_REPLY);

			std::cout << "LOBBY_ENTER_REPLY" << " ";
			if (CLIENT_MGR.RegistClient(socket, playerName))
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
