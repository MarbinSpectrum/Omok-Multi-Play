#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "framework.h"

USING_NS_CC;

class PlayerSlot : public Label
{
public:
    PlayerSlot();
    ~PlayerSlot();

public:
    static  PlayerSlot*     create(std::string pPlayerName, bool pisHost);

public:
    bool                    init(std::string pPlayerName, bool pisHost);
    void                    SetText(std::string pPlayerName);
    void                    SetReady(bool pPlayerReady);
    void                    StartSchedule();

public:
    Size                    slotSize;

private:
    void                    UpdateUI(float f);

private:
    Sprite*                 backGround;
    ui::Text*               playerNameText;
    ui::Text*               playerReadyText;

    bool                    isHost;
    bool                    playerReady;
    std::string             playerName;
};