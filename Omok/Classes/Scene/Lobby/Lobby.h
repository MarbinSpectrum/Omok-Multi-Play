#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "framework.h"

#define CSCENE cocos2d::Scene
#define DIRECTOR Director::getInstance()

USING_NS_CC;

class Lobby : public CSCENE
{
public:
    static CSCENE*  createScene();
    virtual bool    init();
public:
    CREATE_FUNC(Lobby);

};