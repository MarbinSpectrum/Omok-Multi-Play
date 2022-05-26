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

class GameRoom;

#define GAMEROOM_MGR GameRoomMgr::Instance()
typedef __int64 int64;
typedef unsigned int uint;

class GameRoomMgr
{
	typedef std::unordered_map<int64, GameRoom*> RoomData;
	typedef std::unordered_map<uint, RoomData> RoomDataList;
public:
	const static uint MaxRoomNum = 100;
	const static uint MaxPersonNum = 2;

private:
	GameRoomMgr();
	~GameRoomMgr();

private:
	static GameRoomMgr*	instance;

public:
	static GameRoomMgr&	Instance();

public:
	bool				CreateRoom(SOCKET socket);
	bool				RemoveRoom(uint roomNum, int64 roomKey);
	bool				EnterRoom(SOCKET socket, uint roomNum, int64 roomKey);
	void				WriteRoomDatas(Message& message);

private:
	int64				MakeRoomKey();
	uint				MakeRoomNum();
	bool				CreateRoom(int64 roomkey, SOCKET socket);
	GameRoom*			GetGameRoom(uint roomNum, int64 roomKey);
	
private:
	RoomDataList*		roomDataList;
};
