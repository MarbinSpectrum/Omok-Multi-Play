#pragma once

#include "../BaseMgr.h"
#include "framework.h"

#pragma comment(lib, "ws2_32.lib")


class PlayerMgr : BaseMgr
{
public:
	PlayerMgr();
	~PlayerMgr();

public:
	static PlayerMgr&	Instance();
	static void			Destroy();

public:
	virtual void		Init() override;
	void				RegistNickName(std::string nickName);
	std::string*		GetNickName();

private:
	static PlayerMgr*	instance;

private:
	std::string			playerNickName;
	int					playerRoom;
};

