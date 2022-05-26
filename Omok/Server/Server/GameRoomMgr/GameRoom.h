#pragma once

#include "../ClientMgr/ClientMgr.h"
#include "../GameRoomMgr/GameRoomMgr.h"
#include <set>

typedef __int64 int64;
typedef unsigned int uint;

class GameRoom
{
public:
	GameRoom(uint roomNum, int64 roomKey, ClientObj* host);
	~GameRoom();

public:
	uint					CountPlayer();
	bool					ExistRoom(ClientObj* guest);
	bool					EnterGameRoom(ClientObj* guest);
	bool					ExitGameRoom(ClientObj* guest);

private:
	bool					IsHost(ClientObj* guest);


private:
	uint					roomNum;
	int64					roomKey;
	ClientObj*				host;
	std::set<ClientObj*>	clientList;

	bool					gameRun;
};
