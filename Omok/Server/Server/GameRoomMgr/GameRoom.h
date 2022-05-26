#pragma once

#include "../ClientMgr/ClientMgr.h"
#include <set>

typedef __int64 int64;
typedef unsigned int uint;

class GameRoom
{
public:
	GameRoom(uint roomID, int64 roomKey, ClientObj* host);
	~GameRoom();

public:
	uint					CountPlayer();
	bool					ExistRoom(ClientObj* guest);
	bool					EnterGameRoom(ClientObj* guest);

private:
	const uint				maxPerson = 2;

	uint					roomID;
	int64					roomKey;
	ClientObj*				host;
	std::set<ClientObj*>	clientList;

	bool					gameRun;
};
