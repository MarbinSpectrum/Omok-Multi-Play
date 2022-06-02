#pragma once

#include "../../../Message/Message.h"
#include "../ClientMgr/ClientMgr.h"
#include "../GameRoomMgr/GameRoomMgr.h"

#include <iostream>
#include <string>
#include <winsock2.h>

#define MSG_MGR MsgMgr::Instance()

class MsgMgr
{
private:
	static MsgMgr*	instance;

public:
	static MsgMgr&	Instance();

public:
	void			SendMsg(Message message, SOCKET socket);
	void			OnReceiveMsg(Message message,SOCKET socket);
};
