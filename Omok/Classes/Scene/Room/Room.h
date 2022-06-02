#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "framework.h"
#include "PlayerSlot.h"

class PlayerSlot;

#define CSCENE cocos2d::Scene
#define DIRECTOR Director::getInstance()

USING_NS_CC;

class Room : public CSCENE
{
public:
    static CSCENE*  createScene();
    virtual bool    init();
    virtual void    Start() override;
    void            UpdateRoom(std::string pHost, std::string pGuest, bool pReady);

public:
    CREATE_FUNC(Room);

private:
    PlayerSlot* hostSlot;
    PlayerSlot* guestSlot;
};