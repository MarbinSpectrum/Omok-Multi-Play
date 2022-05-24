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
	typedef std::unordered_map<SOCKET, ClientObj> ClientList;
private:
	ClientMgr();
	~ClientMgr();
private:
	static ClientMgr*	instance;

public:
	static ClientMgr&	Instance();

public:
	bool RegistClient(SOCKET socket, std::string playerName)
	{
		if (clientList->find(socket) == clientList->end())
		{
			clientList->insert({ socket,ClientObj(socket,playerName) });
			return true;
		}
		return false;
	}
	void RemoveClient(SOCKET socket)
	{
		if (clientList->find(socket) != clientList->end())
		{
			clientList->erase(socket);
		}
	}

private:
	ClientList* clientList;
};
