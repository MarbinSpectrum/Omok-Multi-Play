#include "GameRoomMgr.h"

GameRoomMgr* GameRoomMgr::instance = NULL;
GameRoomMgr& GameRoomMgr::Instance()
{
	if (instance == NULL)
	{
		//�̱��� ��ü�� �߰�
		instance = new GameRoomMgr();
	}
	return *instance;
}

GameRoomMgr::GameRoomMgr()
: roomDataList(NULL)
{
	roomDataList = new RoomDataList;
}
GameRoomMgr::~GameRoomMgr()
{
	if (roomDataList != NULL)
	{
		for (RoomDataList::iterator iter = roomDataList->begin();
			iter != roomDataList->end(); iter++)
		{
			RoomData& roomData = iter->second;
			for (RoomData::iterator iter2 = roomData.begin();
				iter2 != roomData.end(); iter2++)
			{
				GameRoom* gameRoom = iter2->second;
				if (gameRoom != NULL)
				{
					delete gameRoom;
					gameRoom = NULL;
				}
			}
		}
		delete roomDataList;
		roomDataList = NULL;
	}
}

////////////////////////////////////////////////////////////////////////////////
/// : �� ����
////////////////////////////////////////////////////////////////////////////////
bool GameRoomMgr::CreateRoom(SOCKET socket)
{
	int64 roomKey = MakeRoomKey();
	if (CreateRoom(roomKey, socket))
	{
		//�� �����Ϸ�
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////
/// : �� ����
////////////////////////////////////////////////////////////////////////////////
bool GameRoomMgr::RemoveRoom(uint roomNum, int64 roomKey)
{
	GameRoom* gameRoom = GetGameRoom(roomNum, roomKey);
	if (gameRoom == NULL)
	{
		//�ش� ���� �������� �ʴ�.
		return false;
	}
	RoomData &roomData = (*roomDataList)[roomNum];

	//�������� ����
	roomData.erase(roomKey);
	if (roomData.empty())
	{
		roomDataList->erase(roomNum);
	}

	//����ü�� ����
	delete gameRoom;
	gameRoom = NULL;

	return false;
}

////////////////////////////////////////////////////////////////////////////////
/// : �� ����
////////////////////////////////////////////////////////////////////////////////
bool GameRoomMgr::EnterRoom(SOCKET socket, uint roomNum, int64 roomKey)
{
	GameRoom* gameRoom = GetGameRoom(roomNum, roomKey);
	if (gameRoom == NULL)
	{
		//�ش� ���� �������� �ʴ´�.
		return false;
	}

	ClientObj* clientObj = CLIENT_MGR.GetClient(socket);
	if (clientObj == NULL)
	{
		//�ش� Ŭ���̾�Ʈ�� �������� �ʴ´�.
		return false;
	}

	return gameRoom->EnterGameRoom(clientObj);
}

////////////////////////////////////////////////////////////////////////////////
/// : �� ����
////////////////////////////////////////////////////////////////////////////////
bool GameRoomMgr::ExitRoom(SOCKET socket)
{
	ClientObj* clientObj = CLIENT_MGR.GetClient(socket);
	if (clientObj == NULL)
	{
		//�ش� Ŭ���̾�Ʈ�� �������� �ʴ´�.
		return false;
	}

	uint roomNum = clientObj->playerPos.roomNum;
	int64 roomKey = clientObj->playerPos.roomKey;

	if (roomNum == 0 && roomKey == 0)
	{
		//�÷��̾ �濡 �������� �ʴ´�.
		return true;
	}

	GameRoom* gameRoom = GetGameRoom(roomNum, roomKey);
	if (gameRoom == NULL)
	{
		//�ش� ���� �������� �ʴ´�.
		return false;
	}

	return gameRoom->ExitGameRoom(clientObj);
}

////////////////////////////////////////////////////////////////////////////////
/// : �޼����� �� �������� �ۼ�
////////////////////////////////////////////////////////////////////////////////
void GameRoomMgr::WriteRoomDatas(Message& message)
{
	int roomCnt = roomDataList->size();
	message.WriteMessage(roomCnt);

	for (RoomDataList::iterator iter = roomDataList->begin();
		iter != roomDataList->end(); iter++)
	{
		RoomData& roomData = iter->second;
		for (RoomData::iterator iter2 = roomData.begin();
			iter2 != roomData.end(); iter2++)
		{
			GameRoom* gameRoom = iter2->second;
			gameRoom->WriteLobbyRoomData(message);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
/// : ���� ���� �������� �κ�
////////////////////////////////////////////////////////////////////////////////
GameRoom* GameRoomMgr::GetGameRoom(uint roomNum, int64 roomKey)
{
	if (roomDataList->find(roomNum) == roomDataList->end())
	{
		return NULL;
	}

	RoomData& roomData = (*roomDataList)[roomNum];
	if (roomData.find(roomKey) == roomData.end())
	{
		return NULL;
	}

	GameRoom* gameRoom = roomData[roomKey];
	return gameRoom;
}

////////////////////////////////////////////////////////////////////////////////
/// : ���� Ű�� ����
////////////////////////////////////////////////////////////////////////////////
int64 GameRoomMgr::MakeRoomKey()
{
	//Ű�� ���� �ð� �������� ����
	time_t timer = time(NULL);
	return timer;
}

////////////////////////////////////////////////////////////////////////////////
/// : �� ��ȣ�� ����
////////////////////////////////////////////////////////////////////////////////
uint GameRoomMgr::MakeRoomNum()
{
	for (uint idx = 1; idx <= MaxRoomNum; idx++)
	{
		if (roomDataList->find(idx) == roomDataList->end())
		{
			//���� �ִ� �� ��ȣ�� ����
			return idx;
		}
	}
	//���� �ִ� ���� ������ 0�� ��ȯ
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
/// : ���� �����ϰ� ���
////////////////////////////////////////////////////////////////////////////////
bool GameRoomMgr::CreateRoom(int64 roomkey, SOCKET socket)
{
	if (roomDataList->size() > MaxRoomNum)
	{
		//���� MaxRoomNum�� �ʰ�
		return false;
	}
	
	uint roomNum = MakeRoomNum();
	if (roomNum == 0)
	{
		//���� ������ ����
		return false;
	}

	//���� ������ �÷��̾� ��������
	ClientObj* clientObj = CLIENT_MGR.GetClient(socket);
	clientObj->playerPos = { roomNum,roomkey };

	//�� ���
	roomDataList->insert({ roomNum ,RoomData() });

	//�� ����
	string playerName = clientObj->playerName;
	GameRoom* gameRoom = new GameRoom(playerName, roomNum, roomkey, clientObj);

	//���� ���
	RoomData& roomData = (*roomDataList)[roomNum];
	roomData.insert({ roomkey, gameRoom });

	return true;
}
