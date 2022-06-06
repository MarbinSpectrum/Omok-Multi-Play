#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "framework.h"

USING_NS_CC;

class DontClick : public Label
{
public:
    DontClick();
    ~DontClick();

public:
    static  DontClick*      create();

public:
    bool                    init();
    void                    StartSchedule();
    void                    Update(bool pState);

private:
    void                    UpdateInGame(float f);

private:
    ui::Button*             dontClick;

    bool                    state;
};