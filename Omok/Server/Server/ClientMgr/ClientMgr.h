#pragma once

#include <iostream>
#include <winsock2.h>
#include <string>
#include <unordered_map>

typedef __int64 int64;
typedef unsigned int uint;

#define CLIENT_MGR ClientMgr::Instance()

struct PlayerPos
{
	PlayerPos(uint roomNum, int64 roomKey)
		: roomNum(roomNum)
		, roomKey(roomKey)
	{
	}
	uint roomNum;
	int64 roomKey;
};
struct ClientObj
{
	ClientObj(SOCKET socket, std::string playerName)
	: socket(socket)
	, playerName(playerName)
	, playerPos(0, 0)
	, clientKey(0)
	{
	}
	void ExitRoom()
	{
		playerPos = { 0,0 };
	}

	SOCKET socket;
	std::string playerName;
	PlayerPos playerPos;
	int64 clientKey;
};

class ClientMgr
{
	typedef std::unordered_map<SOCKET, int64> ClientKeyList;
	typedef std::unordered_map<int64, ClientObj*> ClientList;
private:
	ClientMgr();
	~ClientMgr();

private:
	static void			Destory();
	static ClientMgr*	instance;

public:
	static bool			PlayerNameCheck(std::string& playerName);

public:
	static ClientMgr&	Instance();

public:
	bool				RegistClient(SOCKET socket, std::string playerName);
	void				RemoveClient(SOCKET socket);
	ClientObj*			GetClient(SOCKET socket);
	bool				CompareClient(ClientObj* client0, ClientObj* client1);

private:
	int64				MakeClientKey();

private:
	ClientKeyList*		clientKeyList;
	ClientList*			clientList;
};
