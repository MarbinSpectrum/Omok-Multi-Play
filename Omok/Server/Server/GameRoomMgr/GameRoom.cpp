#include "GameRoom.h"

GameRoom::GameRoom(uint roomNum, int64 roomKey, ClientObj* host)
: roomNum(roomNum)
, roomKey(roomKey)
, host(host)
, gameRun(false)
{
}

GameRoom::~GameRoom()
{
}

////////////////////////////////////////////////////////////////////////////////
/// : 현재 방의 인원 수를 가져온다.
////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////
/// : 해당 인원이 방에 존재하는지 검사한다.
////////////////////////////////////////////////////////////////////////////////
bool GameRoom::ExistRoom(ClientObj* guest)
{
	if (guest->socket == host->socket)
	{
		return true;
	}
	for (std::set<ClientObj*>::iterator iter = clientList.begin(); 
		iter != clientList.end(); iter++)
	{
		if (guest->socket == (*iter)->socket)
		{
			return true;
		}
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////
/// : 방에 입장을 요청하고 등록한다.
////////////////////////////////////////////////////////////////////////////////
bool GameRoom::EnterGameRoom(ClientObj* guest)
{
	if (CountPlayer() >= GameRoomMgr::MaxPersonNum)
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

////////////////////////////////////////////////////////////////////////////////
/// : 방을 나가는 것을 요청
////////////////////////////////////////////////////////////////////////////////
bool GameRoom::ExitGameRoom(ClientObj* guest)
{
	if (ExistRoom(guest) == false)
	{
		return false;
	}

	if (IsHost(guest))
	{
		//호스트를 제거
		host = NULL;

		int playerCnt = CountPlayer();
		if (playerCnt == 0)
		{
			//방에 인원이 없다.
			//해당 방을 없애자.
			GAMEROOM_MGR.RemoveRoom(roomNum, roomKey);
		}
		else
		{
			for (std::set<ClientObj*>::iterator iter = clientList.begin();
				iter != clientList.end(); iter++)
			{
				ClientObj* guestClient = (*iter);
				if (guestClient != NULL)
				{
					//아무 인원하나를 방장으로 지정하고
					//해당 클라이언트를 목록에서 제거
					host = guestClient;
					clientList.erase(guestClient);
					return true;
				}
			}
		}
	}
	else
	{
		//해당 인원을 리스트에서 삭제
		clientList.erase(guest);

		int playerCnt = CountPlayer();
		if (playerCnt == 0)
		{
			//방에 인원이 없다.
			//해당 방을 없애자.
			GAMEROOM_MGR.RemoveRoom(roomNum, roomKey);
		}
	}

	return false;
}

bool GameRoom::IsHost(ClientObj* guest)
{
	if (host->socket == guest->socket)
	{
		return true;
	}
	return false;
}
