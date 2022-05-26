#pragma once

#include <iostream>
#include <winsock2.h>
#include <string>
#include <unordered_map>

#define CLIENT_MGR ClientMgr::Instance()

struct ClientObj
{
	ClientObj(SOCKET socket, std::string playerName)
	: socket(socket)
	, playerName(playerName)
	{
	}

	SOCKET socket;
	std::string playerName;
};

class ClientMgr
{
	typedef std::unordered_map<SOCKET, ClientObj*> ClientList;
private:
	ClientMgr();
	~ClientMgr();

private:
	static void Destory();
	static ClientMgr*	instance;

public:
	static ClientMgr&	Instance();

public:
	bool RegistClient(SOCKET socket, std::string playerName);
	void RemoveClient(SOCKET socket);
	ClientObj* GetClient(SOCKET socket);

private:
	ClientList* clientList;
};
