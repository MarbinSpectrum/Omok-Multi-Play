#include "ClientMgr.h"

void ClientMgr::Destory()
{
	delete instance;
}

ClientMgr* ClientMgr::instance = NULL;
ClientMgr& ClientMgr::Instance()
{
	if (instance == NULL)
	{
		atexit(Destory);
		//½Ì±ÛÅæ °´Ã¼¸¦ Ãß°¡
		instance = new ClientMgr();
	}
	return *instance;
}

ClientMgr::ClientMgr()
: clientList(new ClientList)
{
}
ClientMgr::~ClientMgr()
{
	delete clientList;
	clientList = NULL;
}

bool ClientMgr::RegistClient(SOCKET socket, std::string playerName)
{
	if (clientList->find(socket) == clientList->end())
	{
		clientList->insert({ socket,new ClientObj(socket,playerName) });
		return true;
	}
	return false;
}
void ClientMgr::RemoveClient(SOCKET socket)
{
	if (clientList->find(socket) != clientList->end())
	{
		ClientObj* clientObj = GetClient(socket);
		clientList->erase(socket);
		delete clientObj;
		clientObj = NULL;
	}
}
ClientObj* ClientMgr::GetClient(SOCKET socket)
{
	return (*clientList)[socket];
}