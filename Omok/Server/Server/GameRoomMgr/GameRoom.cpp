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
		//�濡 ������ ����.
		return false;
	}

	if (guest == NULL)
	{
		return false;
	}

	if (ExistRoom(guest))
	{
		//�濡 �̹� �����Ѵ�.
		return false;
	}

	if (gameRun)
	{
		//������ �������̴�.
		return false;
	}

	clientList.insert(guest);

	return true;
}