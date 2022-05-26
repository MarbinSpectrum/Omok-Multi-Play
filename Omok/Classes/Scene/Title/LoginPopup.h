#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "framework.h"

#define CSCENE cocos2d::Scene
#define DIRECTOR Director::getInstance()

USING_NS_CC;

class LoginPopup : public Layer
{
public:
    LoginPopup();
    ~LoginPopup();

public:
    CREATE_FUNC(LoginPopup);

    bool    init();
};