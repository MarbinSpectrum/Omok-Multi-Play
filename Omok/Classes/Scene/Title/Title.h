#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "framework.h"

#define CSCENE cocos2d::Scene
#define DIRECTOR Director::getInstance()

USING_NS_CC;

class Title : public CSCENE
{
public:
    static CSCENE*  createScene();
    virtual bool    init();

public:
    void            EnterGameLobby(Ref* pSender);

private:
    ui::TextField*  textField;

public:
    CREATE_FUNC(Title);

};