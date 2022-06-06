#pragma once

#include "../ClientMgr/ClientMgr.h"
#include "../GameRoomMgr/GameRoomMgr.h"
#include "../GameMgr/GameMgr.h"
#include "../MsgMgr/MsgMgr.h"

#include <set>

typedef __int64 int64;
typedef unsigned int uint;

class GameRoom
{
	typedef std::map<int64, bool> ClientReady;
	typedef std::set<ClientObj*> ClientList;
public:
	GameRoom(std::string roomName, uint roomNum, int64 roomKey, ClientObj* host);
	~GameRoom();

public:
	uint					CountPlayer();
	bool					ExistRoom(ClientObj* guest);
	bool					EnterGameRoom(ClientObj* guest);
	bool					ExitGameRoom(ClientObj* guest);
	void					WriteLobbyRoomData(Message& message);
	void					WriteRoomData(Message& message, SOCKET socket);
	bool					GetIsPlayerReady(ClientObj* guest);
	void					SetIsPlayerReady(ClientObj* guest, bool state);
	void					GameStart();
	GameMgr*				GetGameMgr();
	void					BroadCastRoomData(ClientObj* ignore = NULL);

private:
	bool					IsHost(ClientObj* guest);

private:
	std::string					roomName;
	uint						roomNum;
	int64						roomKey;
	ClientObj*					host;
	ClientList					clientList;
	ClientReady					clientReady;
	GameMgr*					gameMgr;
};
