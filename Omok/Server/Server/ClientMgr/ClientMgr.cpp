#include "ClientMgr.h"

ClientMgr* ClientMgr::instance = NULL;
ClientMgr& ClientMgr::Instance()
{
	if (instance == NULL)
	{
		//�̱��� ��ü�� �߰�
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