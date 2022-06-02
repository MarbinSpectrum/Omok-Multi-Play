#pragma once

#include "cocos2d.h"

class BaseMgr
{
public:
	BaseMgr();
	~BaseMgr();

public:
	virtual void Init() = 0;
};