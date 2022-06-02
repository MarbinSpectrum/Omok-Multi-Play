#pragma once

#include "../ClientMgr/ClientMgr.h"
#include "../GameRoomMgr/GameRoomMgr.h"
#include "../MsgMgr/MsgMgr.h"

#include <set>

typedef __int64 int64;
typedef unsigned int uint;

class GameRoom
{
public:
	GameRoom(std::string roomName, uint roomNum, int64 roomKey, ClientObj* host);
	~GameRoom();

public:
	uint					CountPlayer();
	bool					ExistRoom(ClientObj* guest);
	bool					EnterGameRoom(ClientObj* guest);
	bool					ExitGameRoom(ClientObj* guest);
	void					WriteLobbyRoomData(Message& message);
	void					WriteRoomData(Message& message);

private:
	void					BroadCastRoomData(ClientObj* ignore = NULL);
	bool					IsHost(ClientObj* guest);
	bool					IsPlayerReady(ClientObj* guest);

private:
	std::string					roomName;
	uint						roomNum;
	int64						roomKey;
	ClientObj*					host;
	std::set<ClientObj*>		clientList;
	std::map<pair<SOCKET, std::string>, bool>		clientReady;

	bool					gameRun;
};
