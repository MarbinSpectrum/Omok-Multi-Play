#include "Server.h"

Server* Server::instance = NULL;
Server& Server::Instance()
{
	if (instance == NULL)
	{
		//싱글톤 객체를 추가
		instance = new Server();
	}
	return *instance;
}

void Server::Run()
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
	serverSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	//SOCKADDR_IN : 소캣 주소 구조체
	SOCKADDR_IN addr = {};

	//sin_family : 주소 체계를 구분하기 위한 변수
	//AF_INET : IPv4 인터넷 프로토콜
	//항상 이걸로 설정해야한다.
	addr.sin_family = AF_INET;

	//sin_port : 포트번호를 의미하는 변수
	addr.sin_port = htons(4444);

	//sin_addr : 호스트 IP주소
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	//bind : 소캣에 주소를 할당
	bind(serverSock, (SOCKADDR*)&addr, sizeof(addr));

	//listen : 소캣을 연결요청 대기 상태로 만든다
	listen(serverSock, SOMAXCONN);

	std::thread proc(recv_client);

	char buffer[PACKET_SIZE] = { 0 };
	while (!WSAGetLastError())
	{
		//메세지 전송
		//소캣에러가 발생중이지 않을때
	}

	proc.join();

	for (std::set<SOCKET>::iterator iter = clientSock.begin(); iter != clientSock.end(); iter++)
	{
		closesocket(*iter);
	}

	closesocket(serverSock);
	WSACleanup();
}

SOCKLIST* Server::GetSockList()
{
	return &clientSock;
}

CLIENT_THREAD* Server::GetClientThread()
{
	return &clientThread;
}

SOCKET* Server::GetServerSocket()
{
	return &serverSock;
}

////////////////////////////////////////////////////////////////////////////////
/// : socket에 해당하는 클라이언트의 메세지를 지속적으로 받는 쓰레드
////////////////////////////////////////////////////////////////////////////////
void proc_recvs(SOCKET socket)
{
	//메세지를 받을 버퍼
	char buffer[PACKET_SIZE] = { 0 };

	while (!WSAGetLastError())
	{
		ZeroMemory(&buffer, PACKET_SIZE);

		if (!recv(socket, buffer, PACKET_SIZE, 0))
		{
			//메세지를 받을 수 없다고 판단
			break;
		}

		//SOCKLIST* socketList = SERVER.GetSockList();
		//for (SOCKLIST::iterator iter = socketList->begin(); iter != socketList->end(); iter++)
		//{
		//	//다른 클라이언트에게 브로드캐스트
		//	if (*iter != socket)
		//	{
		//		send(*iter, buffer, strlen(buffer), 0);
		//	}
		//}
	}
}

////////////////////////////////////////////////////////////////////////////////
/// : 클라이언트를 지속적으로 받는 쓰레드
////////////////////////////////////////////////////////////////////////////////
void recv_client()
{
	SOCKLIST* socketList = SERVER.GetSockList();
	CLIENT_THREAD* clientThread = SERVER.GetClientThread();

	while (!WSAGetLastError())
	{
		SOCKET clientSock;
		SOCKADDR_IN client = {};
		int client_size = sizeof(client);
		SOCKET serverSock = *SERVER.GetServerSocket();

		//ZeroMemory : client에 client_size만큼 메모리 할당
		ZeroMemory(&client, client_size);

		//클라이언트의 소캣을 받는다.
		clientSock = accept(serverSock, (SOCKADDR*)&client, &client_size);
		if (socketList->find(clientSock) == socketList->end())
		{
			//해당 클라이언트가 없다고 판단되면 등록
			socketList->insert(clientSock);
			(*clientThread)[clientSock] = std::thread(proc_recvs, clientSock);
		}
	}

	for (SOCKLIST::iterator iter = socketList->begin(); iter != socketList->end(); iter++)
	{
		//소켓을 할당할 수 있는 환경이 안될때
		//쓰레드를 해제한다.
		(*clientThread)[*iter].join();
	}
}
