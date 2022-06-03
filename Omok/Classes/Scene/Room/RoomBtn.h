#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "framework.h"

USING_NS_CC;

enum class RoomBtnType
{
    GAME_READY_BTN,
    GAME_READY_OK_BTN,
    GAME_START_BTN,
    GAME_START_NOT_ACT_BTN,
};

class RoomBtn : public Label
{
public:
    RoomBtn();
    ~RoomBtn();


public:
    static  RoomBtn*        create();

public:
    bool                    init();
    void                    SetBtnType(RoomBtnType pRoomBtnType);
    void                    StartSchedule();

public:
    Size                    btnSize;

private:
    void                    UpdateUI(float f);
    void                    ReadyBtn(Ref* ref);
    void                    ReadyOKBtn(Ref* ref);
    void                    GameStartBtn(Ref* ref);

private:
    ui::Button*             readyBtn;
    ui::Button*             readyOKBtn;
    ui::Button*             startBtn;
    cocos2d::Sprite*        startBtnNotAct;

    RoomBtnType             roomBtnType;
};