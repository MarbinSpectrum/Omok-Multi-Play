#include "MessageMgr.h"

MessageMgr::MessageMgr()
{
	//생성자
}

MessageMgr::~MessageMgr()
{
	//소멸자

	//쓰레드를 해제
	receiveThread.~thread();
	closesocket(skt);
	WSACleanup();
}

MessageMgr* MessageMgr::instance = NULL;
MessageMgr& MessageMgr::Instance()
{
	if (instance == NULL)
	{
		//싱글톤 객체를 추가
		instance = new MessageMgr();
	}
	return *instance;
}

void MessageMgr::Destroy()
{
	delete instance;
	instance = NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : 매니저 초기화
////////////////////////////////////////////////////////////////////////////////////////////////
void MessageMgr::Init()
{
	RunThread();
}

////////////////////////////////////////////////////////////////////////////////
/// : 클라이언트에 오는 메세지를 지속적으로 받는 쓰레드
////////////////////////////////////////////////////////////////////////////////
void proc_recv()
{
	char buffer[PACKET_SIZE] = {};
	std::string cmd;
	while (1)
	{
		ZeroMemory(&buffer, PACKET_SIZE);
		recv(MESSAGE_MGR.GetSock(), buffer, PACKET_SIZE, 0);
		cmd = buffer;

		Message message(buffer);

		//메세지 매니저에서 메세지를 처리
		MESSAGE_MGR.OnReceiveMsg(message);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : 쓰레드 실행
////////////////////////////////////////////////////////////////////////////////////////////////
void MessageMgr::RunThread()
{
	//WSADATA 윈도우 소캣 데이터 구조체
	WSADATA wsa;

	//WSAStartup(버전,소캣) : 윈도우 소캣을 초기화 하는 함수
	//MAKEWORD : 매크로함수 (0x0202를 리턴한다)
	WSAStartup(MAKEWORD(2, 2), &wsa);

	//socket(도메인,타입,프로토콜) : 소캣 생성함수
	//PF_INET : IPv4 인터넷 프로토콜을 사용
	//SOCK_STREAM : TCP/IP 프로토콜을 이용
	//IPPROTO_TCP : TCP 프로토콜
	skt = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	//SOCKADDR_IN : 소캣 주소 구조체
	SOCKADDR_IN addr = {};

	//sin_family : 주소 체계를 구분하기 위한 변수
	//AF_INET : IPv4 인터넷 프로토콜
	//항상 이걸로 설정해야한다.
	addr.sin_family = AF_INET;

	//sin_port : 포트번호를 의미하는 변수
	addr.sin_port = htons(4444);

	//sin_addr : 호스트 IP주소
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	while (1)
	{
		//connect : 원격 호스트에 대한 연결
		if (!connect(skt, (SOCKADDR*)&addr, sizeof(addr)))
		{
			break;
		}
	}

	receiveThread = std::thread(proc_recv);
	receiveThread.detach();

	Message message(MessageType::CONNECT_CLIENT);
	message.WriteMessage((int)skt);

	SendMsg(message);
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : 메시지 전송
////////////////////////////////////////////////////////////////////////////////////////////////
void MessageMgr::SendMsg(Message message)
{
	std::string msg = message.ConvertString();
	send(skt, msg.c_str(), strlen(msg.c_str()), 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : 메시지 수신
////////////////////////////////////////////////////////////////////////////////////////////////
void MessageMgr::OnReceiveMsg(Message message)
{
	MessageType messageType = message.GetMessageType();

	switch (messageType)
	{
		case MessageType::LOBBY_ENTER_REPLY:
		{
			int success = std::stoi(message.ReadMessage());

			if (success)
			{
				std::string playerName = message.ReadMessage();
				PLAYER_MGR.RegistNickName(playerName);

				SCENE_MGR.MoveScene("Lobby");
			}
			else
			{
				Scene* scene = SCENE_MGR.GetNowScene();
				Title* titleScene = dynamic_cast<Title*>(scene);

				titleScene->CantLogin();
			}
		}
		break;
		case MessageType::LOBBY_ROOM_DATA_REPLY:
		{
			Lobby::RoomDataList roomList;

			int roomCnt = std::stoi(message.ReadMessage());
			for (int idx = 0; idx < roomCnt; idx++)
			{
				string roomName = message.ReadMessage();
				uint roomNum = StringToUint(message.ReadMessage());
				int64 roomKey = StringToInt64(message.ReadMessage());
				int personNum = std::stoi(message.ReadMessage());
				roomList.push_back(RoomData(roomName,roomNum, roomKey, personNum));
			}

			Scene* scene = SCENE_MGR.GetNowScene();
			Lobby* lobbyScene = dynamic_cast<Lobby*>(scene);
			
			lobbyScene->UpdateLobbyRoomList(&roomList);
		}
		break;
		case MessageType::MAKE_ROOM_REPLY:
		{
			int success = std::stoi(message.ReadMessage());

			if (success)
			{
				SCENE_MGR.MoveScene("Room");
			}
			else
			{
				
			}			
		}
		break;
		case MessageType::ENTER_ROOM_REPLY:
		{
			int success = std::stoi(message.ReadMessage());

			if (success)
			{
				SCENE_MGR.MoveScene("Room");
			}
			else
			{
				Scene* scene = SCENE_MGR.GetNowScene();
				Lobby* lobbyScene = dynamic_cast<Lobby*>(scene);

				lobbyScene->CantRoomEnter();
			}
		}
		break;
		case MessageType::EXIT_ROOM_REPLY:
		{
			int success = std::stoi(message.ReadMessage());

			if (success)
			{
				SCENE_MGR.MoveScene("Lobby");
			}
			else
			{

			}
		}
		break;
		case MessageType::GAMEROOM_DATA_REPLY:
		{
			int success = std::stoi(message.ReadMessage());

			if (success)
			{
				Scene* scene = SCENE_MGR.GetNowScene();
				Room* roomScene = dynamic_cast<Room*>(scene);

				std::string host = message.ReadMessage();
				bool isHost = stoi(message.ReadMessage());
				int cnt = stoi(message.ReadMessage());

				if (roomScene != NULL)
				{
					if (cnt == 0)
					{
						roomScene->UpdateRoom(host, "", false, isHost);
					}
					else
					{
						std::string guest = message.ReadMessage();
						bool ready = std::stoi(message.ReadMessage());
						roomScene->UpdateRoom(host, guest, ready, isHost);
					}
				}
			}
			else
			{

			}
		}
		break;
		case MessageType::GAMEROOM_GAME_START_REPLY:
		{
			int success = std::stoi(message.ReadMessage());

			if (success)
			{
				SCENE_MGR.MoveScene("InGame");
			}
			else
			{

			}
		}
		break;
		case MessageType::GAMEBOARD_DATA_REPLY:
		{
			int success = std::stoi(message.ReadMessage());

			if (success)
			{
				Scene* scene = SCENE_MGR.GetNowScene();
				InGame* inGameScene = dynamic_cast<InGame*>(scene);

				inGameScene->UpdateGameBoard(message);

			}
			else
			{

			}
		}
		break;
		case MessageType::GAME_RESULT_REPLY:
		{
			int success = std::stoi(message.ReadMessage());

			if (success)
			{
				Scene* scene = SCENE_MGR.GetNowScene();
				InGame* inGameScene = dynamic_cast<InGame*>(scene);

				inGameScene->UpdateGameResult(message);

			}
			else
			{

			}
		}
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : 클라이언트 소캣
////////////////////////////////////////////////////////////////////////////////////////////////
SOCKET MessageMgr::GetSock()
{
	return skt;
}
