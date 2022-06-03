#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "framework.h"

#define CSCENE cocos2d::Scene
#define DIRECTOR Director::getInstance()

USING_NS_CC;

class RoomEnterPopup : public Layer
{
public:
    RoomEnterPopup();
    ~RoomEnterPopup();

public:
    CREATE_FUNC(RoomEnterPopup);

    void    ClosePopUp(Ref* ref);
    bool    init();
};