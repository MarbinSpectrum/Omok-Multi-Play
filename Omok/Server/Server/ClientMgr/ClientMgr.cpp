#include "ClientMgr.h"
#include "../GameRoomMgr/GameRoomMgr.h"

void ClientMgr::Destory()
{
	delete instance;
}

ClientMgr* ClientMgr::instance = NULL;
ClientMgr& ClientMgr::Instance()
{
	if (instance == NULL)
	{
		atexit(Destory);
		//�̱��� ��ü�� �߰�
		instance = new ClientMgr();
	}
	return *instance;
}

bool ClientMgr::PlayerNameCheck(std::string& playerName)
{
	if (playerName.size() > 8)
		return false;

	for (int i = 0; i < playerName.size(); i++)
	{
		if ('a' <= playerName[i] && playerName[i] <= 'z')
			continue;
		if ('0' <= playerName[i] && playerName[i] <= '9')
			continue;
		if ('A' <= playerName[i] && playerName[i] <= 'Z')
			continue;
		return false;
	}
	return true;
}

ClientMgr::ClientMgr()
: clientList(new ClientList)
{
}
ClientMgr::~ClientMgr()
{
	delete clientList;
	clientList = NULL;
}

bool ClientMgr::RegistClient(SOCKET socket, std::string playerName)
{
	if (clientList->find(socket) == clientList->end())
	{
		clientList->insert({ socket,new ClientObj(socket,playerName) });
		return true;
	}
	return false;
}
void ClientMgr::RemoveClient(SOCKET socket)
{
	if (clientList->find(socket) != clientList->end())
	{
		ClientObj* clientObj = GetClient(socket);
		
		if (clientObj == NULL)
		{
			return;
		}

		//Ŭ���̾�Ʈ ������ ����Ʈ���� ����
		clientList->erase(socket);
		
		//�ش��ο��� ���� �ִ� ���� Ž��
		uint roomNum = clientObj->playerPos.roomNum;
		int64 roomKey = clientObj->playerPos.roomKey;
		GameRoom* gameRoom = GAMEROOM_MGR.GetGameRoom(roomNum, roomKey);
		if (gameRoom != NULL)
		{
			//�濡�� �÷��̾� ����
			gameRoom->ExitGameRoom(clientObj);
		}

		delete clientObj;
		clientObj = NULL;
	}
}
ClientObj* ClientMgr::GetClient(SOCKET socket)
{
	return (*clientList)[socket];
}