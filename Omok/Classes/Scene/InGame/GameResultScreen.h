#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "framework.h"

USING_NS_CC;

enum class GameResult
{
    EMPTY = 0,
    VICTORY = 1,
    DEFEAT  = 2,
    DRAW    = 100,
};

class GameResultScreen : public Label
{
public:
    GameResultScreen();
    ~GameResultScreen();

public:
    static  GameResultScreen*       create();

public:
    bool                            init();
    void                            StartSchedule();
    void                            Update(GameResult pGameresult);

private:
    void                            UpdateInGame(float f);
    void                            RoomExit(Ref* ref);

private:
    cocos2d::Sprite*                victoryScreen;
    cocos2d::Sprite*                defeatScreen;
    cocos2d::Sprite*                drawScreen;
    ui::Button*                     exitBnt;

    GameResult                      gameResult;
};