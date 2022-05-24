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

bool GameRoomMgr::CreateRoom()
{
	int64 roomKey = MakeRoomKey();
	if (CreateRoom(roomKey))
	{
		//�� �����Ϸ�
		return true;
	}
	return false;
}

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
			uint roomNum = iter->first;
			int64 roomKey = iter2->first;
			
			GameRoom* gameRoom = iter2->second;
			int personNum = gameRoom->CountPlayer();

			message.WriteMessage(UintToString(roomNum));
			message.WriteMessage(Int64ToString(roomKey));
			message.WriteMessage(to_string(personNum));
		}
	}
}

int64 GameRoomMgr::MakeRoomKey()
{
	time_t timer = time(NULL);
	return timer;
}

uint GameRoomMgr::MakeRoomID()
{
	for (uint idx = 1; idx <= MaxRoomNum; idx++)
	{
		if (roomDataList->find(idx) == roomDataList->end())
		{
			return idx;
		}
	}
	return 0;
}

bool GameRoomMgr::CreateRoom(int64 roomkey)
{
	if (roomDataList->size() > MaxRoomNum)
	{
		//���� MaxRoomNum�� �ʰ�
		return false;
	}
	
	uint roomID = MakeRoomID();
	if (roomID == 0)
	{
		//���� ������ ����
		return false;
	}

	//�� ���
	roomDataList->insert({ roomID ,RoomData() });

	RoomData& roomData = roomDataList->find(roomID)->second;
	roomData.insert({ roomkey ,new GameRoom() });

	return true;
}