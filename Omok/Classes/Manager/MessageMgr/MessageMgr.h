#pragma once

#include "../../Message/Message.h"
#include "../BaseMgr.h"
#include "framework.h"

#include <winsock2.h> 
#include <WS2tcpip.h>
#include <iostream> 
#include <thread>

#pragma comment(lib, "ws2_32.lib")

#define PACKET_SIZE 1024 

typedef __int64 int64;
typedef unsigned int uint;

void proc_recv();

class MessageMgr : BaseMgr
{
public:
	MessageMgr();
	~MessageMgr();

public:
	static MessageMgr&	Instance();
	static void			Destroy();

public:
	virtual void		Init() override;
	void				RunThread();
	void				SendMsg(Message message);
	void				OnReceiveMsg(Message message);
	SOCKET				GetSock();

private:
	static MessageMgr*	instance;

private:
	SOCKET				skt;
	std::thread			receiveThread;
};

