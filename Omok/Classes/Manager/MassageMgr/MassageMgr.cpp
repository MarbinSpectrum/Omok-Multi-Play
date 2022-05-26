#include "MassageMgr.h"

MassageMgr::MassageMgr()
{
	//생성자
}

MassageMgr::~MassageMgr()
{
	//소멸자

	//쓰레드를 해제
	receiveThread.join();
	closesocket(skt);
	WSACleanup();
}

MassageMgr* MassageMgr::instance = NULL;
MassageMgr& MassageMgr::Instance()
{
	if (instance == NULL)
	{
		//싱글톤 객체를 추가
		instance = new MassageMgr();
	}
	return *instance;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : 매니저 초기화
////////////////////////////////////////////////////////////////////////////////////////////////
void MassageMgr::Init()
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
		recv(MASSAGE_MGR.GetSock(), buffer, PACKET_SIZE, 0);
		cmd = buffer;

		Message message(buffer);

		//메세지 매니저에서 메세지를 처리
		MASSAGE_MGR.OnReceiveMsg(message);
	}
}
void MassageMgr::RunThread()
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

	Message message(MessageType::CONNECT_CLIENT);
	message.WriteMessage((int)skt);

	SendMsg(message);
}

void MassageMgr::SendMsg(Message message)
{
	std::string msg = message.ConvertString();
	send(skt, msg.c_str(), strlen(msg.c_str()), 0);
}

void MassageMgr::OnReceiveMsg(Message message)
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
				//접속 실패
			}
		}
		break;
		case MessageType::LOBBY_ROOM_DATA_REPLY:
		{
			Lobby::RoomDataList roomList;

			int roomCnt = std::stoi(message.ReadMessage());
			for (int idx = 0; idx < roomCnt; idx++)
			{
				uint roomNum = StringToUint(message.ReadMessage());
				int64 roomKey = StringToInt64(message.ReadMessage());
				int personNum = std::stoi(message.ReadMessage());
				roomList.push_back(RoomData(roomNum, roomKey, personNum));
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

			}
		}
		break;
	}
}

SOCKET MassageMgr::GetSock()
{
	return skt;
}
