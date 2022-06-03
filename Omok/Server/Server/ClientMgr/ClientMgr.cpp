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
: clientKeyList(new ClientKeyList)
, clientList(new ClientList)
{
}
ClientMgr::~ClientMgr()
{
	delete clientList;
	clientList = NULL;
}

bool ClientMgr::RegistClient(SOCKET socket, std::string playerName)
{
	if (clientKeyList->find(socket) == clientKeyList->end())
	{
		int64 key = MakeClientKey();
		if (clientList->find(key) == clientList->end())
		{
			ClientObj* client = new ClientObj(socket, playerName);
			client->clientKey = key;

			clientKeyList->insert({ socket,key });
			clientList->insert({ key,client });
			return true;
		}
	}
	return false;
}
void ClientMgr::RemoveClient(SOCKET socket)
{
	ClientObj* clientObj = GetClient(socket);

	if (clientObj == NULL)
	{
		return;
	}

	//클라이언트 정보를 리스트에서 삭제
	//키값 제거
	//클라이언트 제거
	int64 key = (*clientKeyList)[socket];
	clientKeyList->erase(socket);
	clientList->erase(key);

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
ClientObj* ClientMgr::GetClient(SOCKET socket)
{
	int64 key = (*clientKeyList)[socket];
	return (*clientList)[key];
}

bool ClientMgr::CompareClient(ClientObj* client0, ClientObj* client1)
{
	if (client0 != NULL && client1 != NULL)
	{
		return (client0->clientKey == client1->clientKey && 
			client0->playerName.compare(client0->playerName) == 0);
	}
	return false;
}

int64 ClientMgr::MakeClientKey()
{
	//키는 현재 시간 기준으로 생성
	time_t timer = time(NULL);
	return timer;
}
