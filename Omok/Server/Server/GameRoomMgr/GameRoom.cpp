#include "GameRoom.h"

GameRoom::GameRoom(uint roomID, int64 roomKey, ClientObj* host)
: roomID(roomID)
, roomKey(roomKey)
, host(host)
{
}

GameRoom::~GameRoom()
{
}

uint GameRoom::CountPlayer()
{
	uint num = 0;
	if (host != NULL)
	{
		num++;
	}
	for (std::set<ClientObj*>::iterator iter = clientList.begin(); iter != clientList.end(); iter++)
	{
		if (*iter != NULL)
		{
			num++;
		}
	}
	return num;
}

bool GameRoom::ExistRoom(ClientObj* guest)
{
	if (guest->socket == host->socket)
	{
		return true;
	}
	for (std::set<ClientObj*>::iterator iter = clientList.begin(); iter != clientList.end(); iter++)
	{
		if (guest->socket == (*iter)->socket)
		{
			return true;
		}
	}
	return false;
}

bool GameRoom::EnterGameRoom(ClientObj* guest)
{
	if (CountPlayer() >= maxPerson)
	{
		//방에 공간이 없다.
		return false;
	}

	if (guest == NULL)
	{
		return false;
	}

	if (ExistRoom(guest))
	{
		//방에 이미 존재한다.
		return false;
	}

	if (gameRun)
	{
		//게임이 진행중이다.
		return false;
	}

	clientList.insert(guest);

	return true;
}