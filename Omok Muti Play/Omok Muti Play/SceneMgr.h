#pragma once

#include "BaseMgr.h"

class SceneMgr : BaseMgr
{
public:
	SceneMgr();
	~SceneMgr();

public:
	virtual void Init() override;
};

