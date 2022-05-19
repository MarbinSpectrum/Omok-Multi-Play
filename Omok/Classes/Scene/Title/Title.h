#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#define CSCENE cocos2d::Scene
#define DIRECTOR Director::getInstance()

class Title : public CSCENE
{
public:
    static CSCENE* createScene();
    virtual bool    init();

public:
    void            EnterGameLobby(cocos2d::Ref* pSender);
    CREATE_FUNC(Title);

};