#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "framework.h"
#include "LoginPopup.h"

class LoginPopup;

#define CSCENE cocos2d::Scene
#define DIRECTOR Director::getInstance()

USING_NS_CC;

class Title : public CSCENE
{
public:
    static CSCENE*  createScene();
    virtual bool    init();
    virtual void    Start() override;

public:
    void            EnterGameLobby(Ref* pSender);
    void            CantLogin();

private:
    ui::TextField*  textField;
    LoginPopup*     loginPopup;

public:
    CREATE_FUNC(Title);

};