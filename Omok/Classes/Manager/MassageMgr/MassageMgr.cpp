#include "MassageMgr.h"

MassageMgr::MassageMgr()
{
	//������
}

MassageMgr::~MassageMgr()
{
	//�Ҹ���

	//�����带 ����
	receiveThread.join();
	closesocket(skt);
	WSACleanup();
}

MassageMgr* MassageMgr::instance = NULL;
MassageMgr& MassageMgr::Instance()
{
	if (instance == NULL)
	{
		//�̱��� ��ü�� �߰�
		instance = new MassageMgr();
	}
	return *instance;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : �Ŵ��� �ʱ�ȭ
////////////////////////////////////////////////////////////////////////////////////////////////
void MassageMgr::Init()
{
	RunThread();
}

////////////////////////////////////////////////////////////////////////////////
/// : Ŭ���̾�Ʈ�� ���� �޼����� ���������� �޴� ������
////////////////////////////////////////////////////////////////////////////////
void proc_recv()
{
	char buffer[PACKET_SIZE] = {};
	std::string cmd;
	while (!WSAGetLastError())
	{
		ZeroMemory(&buffer, PACKET_SIZE);
		recv(MASSAGE_MGR.GetSock(), buffer, PACKET_SIZE, 0);
		cmd = buffer;
	}
}
void MassageMgr::RunThread()
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
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	while (1)
	{
		//connect : ���� ȣ��Ʈ�� ���� ����
		if (!connect(skt, (SOCKADDR*)&addr, sizeof(addr)))
		{
			break;
		}
	}

	receiveThread = std::thread(proc_recv);

	Message message(MessageType::CONNECT_CLIENT);
	message.WriteMessage(skt);

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
			
		}
		break;
	}
}

SOCKET MassageMgr::GetSock()
{
	return skt;
}
