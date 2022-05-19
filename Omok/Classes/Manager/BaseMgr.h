#pragma once

class BaseMgr
{
public:
	BaseMgr();
	~BaseMgr();

public:
	virtual void Init() = 0;
};

