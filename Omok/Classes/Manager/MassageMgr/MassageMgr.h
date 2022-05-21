#pragma once

#include "../../Message/Message.h"
#include "../BaseMgr.h"

class MassageMgr : BaseMgr
{
public:
	MassageMgr();
	~MassageMgr();

private:
	static MassageMgr* instance;

public:
	static MassageMgr& Instance();

public:
	virtual void	Init() override;
	void			SendMessage(Message message);
};

