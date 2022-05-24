#pragma once

#include "../BaseMgr.h"
#include "framework.h"

#pragma comment(lib, "ws2_32.lib")


class PlayerMgr : BaseMgr
{
public:
	PlayerMgr();
	~PlayerMgr();

private:
	static PlayerMgr*	instance;

public:
	static PlayerMgr&	Instance();

public:
	virtual void		Init() override;

public:
	void				RegistNickName(std::string nickName);
	std::string*		GetNickName();
private:
	std::string playerNickName;
	int playerRoom;
};

