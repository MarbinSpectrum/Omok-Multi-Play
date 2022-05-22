#pragma once

#include "../../../Message/Message.h"
#include <iostream>

#define MAG_MGR MsgMgr::Instance()

class MsgMgr
{
private:
	static MsgMgr*	instance;

public:
	static MsgMgr&	Instance();

public:
	void			SendMsg(Message message);
	void			OnReceiveMsg(Message message);
};
