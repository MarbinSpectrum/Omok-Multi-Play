#include "Server.h"

Server* Server::instance = NULL;
Server& Server::Instance()
{
	if (instance == NULL)
	{
		//�̱��� ��ü�� �߰�
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
			//�޼����� ���� �� ���ٰ� �Ǵ�
			break;
		}

		for (SOCKLIST::iterator iter = socketList->begin(); iter != socketList->end(); iter++)
		{
			//�ٸ� Ŭ���̾�Ʈ���� ��ε�ĳ��Ʈ
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

		//ZeroMemory : client�� client_size��ŭ �޸� �Ҵ�
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
	//WSADATA ������ ��Ĺ ������ ����ü
	WSADATA wsa;

	//WSAStartup(����,��Ĺ) : ������ ��Ĺ�� �ʱ�ȭ �ϴ� �Լ�
	//MAKEWORD : ��ũ���Լ� (0x0202�� �����Ѵ�)
	WSAStartup(MAKEWORD(2, 2), &wsa);

	//socket(������,Ÿ��,��������) : ��Ĺ �����Լ�
	//PF_INET : IPv4 ���ͳ� ���������� ���
	//SOCK_STREAM : TCP/IP ���������� �̿�
	//IPPROTO_TCP : TCP ��������
	skt = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	//SOCKADDR_IN : ��Ĺ �ּ� ����ü
	SOCKADDR_IN addr = {};

	//sin_family : �ּ� ü�踦 �����ϱ� ���� ����
	//AF_INET : IPv4 ���ͳ� ��������
	//�׻� �̰ɷ� �����ؾ��Ѵ�.
	addr.sin_family = AF_INET;

	//sin_port : ��Ʈ��ȣ�� �ǹ��ϴ� ����
	addr.sin_port = htons(4444);

	//sin_addr : ȣ��Ʈ IP�ּ�
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	//bind : ��Ĺ�� �ּҸ� �Ҵ�
	bind(skt, (SOCKADDR*)&addr, sizeof(addr));

	//listen : ��Ĺ�� �����û ��� ���·� �����
	listen(skt, SOMAXCONN);

	std::thread proc(recv_client);

	char buffer[PACKET_SIZE] = { 0 };
	while (!WSAGetLastError())
	{
		//�޼��� ����
		//��Ĺ������ �߻������� ������
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
