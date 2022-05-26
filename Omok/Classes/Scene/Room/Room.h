#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "framework.h"

#define CSCENE cocos2d::Scene
#define DIRECTOR Director::getInstance()

USING_NS_CC;

class Room : public CSCENE
{
public:
    static CSCENE*  createScene();
    virtual bool    init();
    virtual void    Start() override;

public:
    CREATE_FUNC(Room);


private:
    ui::Text* playerName0;
    ui::Text* playerName1;
};