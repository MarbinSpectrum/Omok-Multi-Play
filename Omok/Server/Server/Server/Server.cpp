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
	serverSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

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
	bind(serverSock, (SOCKADDR*)&addr, sizeof(addr));

	//listen : ��Ĺ�� �����û ��� ���·� �����
	listen(serverSock, SOMAXCONN);

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
/// : socket�� �ش��ϴ� Ŭ���̾�Ʈ�� �޼����� ���������� �޴� ������
////////////////////////////////////////////////////////////////////////////////
void proc_recvs(SOCKET socket)
{
	//�޼����� ���� ����
	char buffer[PACKET_SIZE] = { 0 };

	while (!WSAGetLastError())
	{
		ZeroMemory(&buffer, PACKET_SIZE);

		if (!recv(socket, buffer, PACKET_SIZE, 0))
		{
			//�޼����� ���� �� ���ٰ� �Ǵ�
			break;
		}

		//SOCKLIST* socketList = SERVER.GetSockList();
		//for (SOCKLIST::iterator iter = socketList->begin(); iter != socketList->end(); iter++)
		//{
		//	//�ٸ� Ŭ���̾�Ʈ���� ��ε�ĳ��Ʈ
		//	if (*iter != socket)
		//	{
		//		send(*iter, buffer, strlen(buffer), 0);
		//	}
		//}
	}
}

////////////////////////////////////////////////////////////////////////////////
/// : Ŭ���̾�Ʈ�� ���������� �޴� ������
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

		//ZeroMemory : client�� client_size��ŭ �޸� �Ҵ�
		ZeroMemory(&client, client_size);

		//Ŭ���̾�Ʈ�� ��Ĺ�� �޴´�.
		clientSock = accept(serverSock, (SOCKADDR*)&client, &client_size);
		if (socketList->find(clientSock) == socketList->end())
		{
			//�ش� Ŭ���̾�Ʈ�� ���ٰ� �ǴܵǸ� ���
			socketList->insert(clientSock);
			(*clientThread)[clientSock] = std::thread(proc_recvs, clientSock);
		}
	}

	for (SOCKLIST::iterator iter = socketList->begin(); iter != socketList->end(); iter++)
	{
		//������ �Ҵ��� �� �ִ� ȯ���� �ȵɶ�
		//�����带 �����Ѵ�.
		(*clientThread)[*iter].join();
	}
}
