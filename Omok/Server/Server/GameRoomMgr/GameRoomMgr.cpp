#include "GameRoomMgr.h"

GameRoomMgr* GameRoomMgr::instance = NULL;
GameRoomMgr& GameRoomMgr::Instance()
{
	if (instance == NULL)
	{
		//싱글톤 객체를 추가
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
/// : 방 생성
////////////////////////////////////////////////////////////////////////////////
bool GameRoomMgr::CreateRoom(SOCKET socket)
{
	int64 roomKey = MakeRoomKey();
	if (CreateRoom(roomKey, socket))
	{
		//방 생성완료
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////
/// : 방 제거
////////////////////////////////////////////////////////////////////////////////
bool GameRoomMgr::RemoveRoom(uint roomNum, int64 roomKey)
{
	GameRoom* gameRoom = GetGameRoom(roomNum, roomKey);
	if (gameRoom == NULL)
	{
		//해당 방이 존재하지 않다.
		return false;
	}
	RoomData &roomData = (*roomDataList)[roomNum];

	//방정보를 제거
	roomData.erase(roomKey);
	if (roomData.empty())
	{
		roomDataList->erase(roomNum);
	}

	//방자체를 삭제
	delete gameRoom;
	gameRoom = NULL;

	return false;
}

////////////////////////////////////////////////////////////////////////////////
/// : 방 입장
////////////////////////////////////////////////////////////////////////////////
bool GameRoomMgr::EnterRoom(SOCKET socket, uint roomNum, int64 roomKey)
{
	GameRoom* gameRoom = GetGameRoom(roomNum, roomKey);
	if (gameRoom == NULL)
	{
		//해당 방이 존재하지 않는다.
		return false;
	}

	ClientObj* clientObj = CLIENT_MGR.GetClient(socket);
	if (clientObj == NULL)
	{
		//해당 클라이언트가 존재하지 않는다.
		return false;
	}

	return gameRoom->EnterGameRoom(clientObj);
}

////////////////////////////////////////////////////////////////////////////////
/// : 메세지에 방 정보들을 작성
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

////////////////////////////////////////////////////////////////////////////////
/// : 방의 키를 생성
////////////////////////////////////////////////////////////////////////////////
int64 GameRoomMgr::MakeRoomKey()
{
	//키는 현재 시간 기준으로 생성
	time_t timer = time(NULL);
	return timer;
}

////////////////////////////////////////////////////////////////////////////////
/// : 방 번호를 생성
////////////////////////////////////////////////////////////////////////////////
uint GameRoomMgr::MakeRoomNum()
{
	for (uint idx = 1; idx <= MaxRoomNum; idx++)
	{
		if (roomDataList->find(idx) == roomDataList->end())
		{
			//남아 있는 방 번호를 배정
			return idx;
		}
	}
	//남아 있는 방이 없으면 0을 반환
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
/// : 방을 생성하고 등록
////////////////////////////////////////////////////////////////////////////////
bool GameRoomMgr::CreateRoom(int64 roomkey, SOCKET socket)
{
	if (roomDataList->size() > MaxRoomNum)
	{
		//방이 MaxRoomNum를 초과
		return false;
	}
	
	uint roomNum = MakeRoomNum();
	if (roomNum == 0)
	{
		//방의 공간이 부족
		return false;
	}

	//방 등록
	roomDataList->insert({ roomNum ,RoomData() });

	//방 생성
	ClientObj* clientObj = CLIENT_MGR.GetClient(socket);
	GameRoom* gameRoom = new GameRoom(roomNum, roomkey, clientObj);

	//방을 등록
	RoomData& roomData = (*roomDataList)[roomNum];
	roomData.insert({ roomkey, gameRoom });

	return true;
}

////////////////////////////////////////////////////////////////////////////////
/// : 게임 방을 가져오는 부분
////////////////////////////////////////////////////////////////////////////////
GameRoom* GameRoomMgr::GetGameRoom(uint roomNum, int64 roomKey)
{
	if (roomDataList->find(roomNum) == roomDataList->end())
	{
		return NULL;
	}

	RoomData &roomData = (*roomDataList)[roomNum];
	if (roomData.find(roomKey) == roomData.end())
	{
		return NULL;
	}

	GameRoom* gameRoom = roomData[roomKey];
	return gameRoom;
}