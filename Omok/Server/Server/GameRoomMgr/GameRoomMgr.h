#pragma once

#include "../ClientMgr/ClientMgr.h"
#include "../../../Message/Message.h"
#include "../../../Algorithm/Algorithm.h"
#include "GameRoom.h"

#include <iostream>
#include <string>
#include <set>
#include <unordered_map>
#include <winsock2.h>
#include <ctime>

#define GAMEROOM_MGR GameRoomMgr::Instance()
typedef __int64 int64;
typedef unsigned int uint;

class GameRoomMgr
{
	const int64 MaxRoomNum = 100;
	typedef std::unordered_map<int64, GameRoom*> RoomData;
	typedef std::unordered_map<uint, RoomData> RoomDataList;
private:
	GameRoomMgr();
	~GameRoomMgr();
private:
	static GameRoomMgr*	instance;

public:
	static GameRoomMgr&	Instance();

public:
	bool				CreateRoom();
	void				WriteRoomDatas(Message& message);
private:
	int64				MakeRoomKey();
	uint				MakeRoomID();
	bool				CreateRoom(int64 roomNum);
	
private:
	RoomDataList*		roomDataList;
};
