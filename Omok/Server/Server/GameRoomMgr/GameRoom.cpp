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
/// : ���� ���� �ο� ���� �����´�.
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
/// : �ش� �ο��� �濡 �����ϴ��� �˻��Ѵ�.
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
/// : �濡 ������ ��û�ϰ� ����Ѵ�.
////////////////////////////////////////////////////////////////////////////////
bool GameRoom::EnterGameRoom(ClientObj* guest)
{
	if (CountPlayer() >= GameRoomMgr::MaxPersonNum)
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

////////////////////////////////////////////////////////////////////////////////
/// : ���� ������ ���� ��û
////////////////////////////////////////////////////////////////////////////////
bool GameRoom::ExitGameRoom(ClientObj* guest)
{
	if (ExistRoom(guest) == false)
	{
		return false;
	}

	if (IsHost(guest))
	{
		//ȣ��Ʈ�� ����
		host = NULL;

		int playerCnt = CountPlayer();
		if (playerCnt == 0)
		{
			//�濡 �ο��� ����.
			//�ش� ���� ������.
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
					//�ƹ� �ο��ϳ��� �������� �����ϰ�
					//�ش� Ŭ���̾�Ʈ�� ��Ͽ��� ����
					host = guestClient;
					clientList.erase(guestClient);
					return true;
				}
			}
		}
	}
	else
	{
		//�ش� �ο��� ����Ʈ���� ����
		clientList.erase(guest);

		int playerCnt = CountPlayer();
		if (playerCnt == 0)
		{
			//�濡 �ο��� ����.
			//�ش� ���� ������.
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
