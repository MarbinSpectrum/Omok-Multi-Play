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

void proc_recvs(SOCKET socket)
{
	char buffer[PACKET_SIZE] = { 0 };
	SOCKLIST* socketList = SERVER.GetSockList();

	while (!WSAGetLastError())
	{
		ZeroMemory(&buffer, PACKET_SIZE);

		if (!recv(socket, buffer, PACKET_SIZE, 0))
		{
			//메세지를 받을 수 없다고 판단
			break;
		}

		for (SOCKLIST::iterator iter = socketList->begin(); iter != socketList->end(); iter++)
		{
			//다른 클라이언트에게 브로드캐스트
			if (*iter != socket)
			{
				send(*iter, buffer, strlen(buffer), 0);
			}
		}
	}
}
void recv_client()
{
	SOCKLIST* socketList = SERVER.GetSockList();
	CLIENT_THREAD* clientThread = SERVER.GetClientThread();
	while (1)
	{
		int client_size = sizeof(client);

		//ZeroMemory : client에 client_size만큼 메모리 할당
		ZeroMemory(&client, client_size);
		client_sock = accept(skt, (SOCKADDR*)&client, &client_size);
		if (socketList->find(client_sock) == socketList->end())
		{
			socketList->insert(client_sock);
			(*clientThread)[client_sock] = std::thread(proc_recvs, client_sock);
		}
	}

	for (SOCKLIST::iterator iter = socketList->begin(); iter != socketList->end(); iter++)
	{
		(*clientThread)[*iter].join();
	}
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
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	//bind : 소캣에 주소를 할당
	bind(skt, (SOCKADDR*)&addr, sizeof(addr));

	//listen : 소캣을 연결요청 대기 상태로 만든다
	listen(skt, SOMAXCONN);

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

	closesocket(skt);
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
