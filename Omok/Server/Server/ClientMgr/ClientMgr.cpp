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
		//싱글톤 객체를 추가
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

		//클라이언트 정보를 리스트에서 삭제
		clientList->erase(socket);
		
		//해당인원에 속해 있는 방을 탐색
		uint roomNum = clientObj->playerPos.roomNum;
		int64 roomKey = clientObj->playerPos.roomKey;
		GameRoom* gameRoom = GAMEROOM_MGR.GetGameRoom(roomNum, roomKey);
		if (gameRoom != NULL)
		{
			//방에서 플레이어 제거
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