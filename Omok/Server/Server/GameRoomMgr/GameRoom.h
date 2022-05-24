#pragma once

#include "../ClientMgr/ClientMgr.h"

typedef unsigned int uint;

class GameRoom
{
public:
	GameRoom();
	~GameRoom();

public:
	uint CountPlayer();

private:
	uint roomID;
	bool gameRun;
	ClientObj* player0;
	ClientObj* player1;
};
