#include "GameResultScreen.h"

USING_NS_CC;

GameResultScreen::GameResultScreen()
: victoryScreen(NULL)
, defeatScreen(NULL)
, drawScreen(NULL)
, gameResult(GameResult::EMPTY)
{
}

GameResultScreen::~GameResultScreen()
{
    this->unschedule(CC_SCHEDULE_SELECTOR(GameResultScreen::UpdateInGame));
}

GameResultScreen* GameResultScreen::create()
{
    GameResultScreen* ret = new GameResultScreen();
    if (ret && ret->init())
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool GameResultScreen::init()
{
    auto visibleSize = DIRECTOR->getVisibleSize();
    Vec2 origin = DIRECTOR->getVisibleOrigin();

    victoryScreen = cocos2d::Sprite::create("res/Victory.png");
    victoryScreen->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
    if(victoryScreen != NULL)
    {
        auto dontClick = ui::Button::create("res/Null.png");
        dontClick->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
        dontClick->setScale(visibleSize.width, visibleSize.height);
        victoryScreen->addChild(dontClick);

        auto fadeBack = LayerColor::create(Color4B(0, 0, 0, 100),
            visibleSize.width, visibleSize.height);
        victoryScreen->addChild(fadeBack);

        this->addChild(victoryScreen, 0);
        victoryScreen->setVisible(false);
    }

    defeatScreen = cocos2d::Sprite::create("res/Defeat.png");
    defeatScreen->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
    if (defeatScreen != NULL)
    {
        auto dontClick = ui::Button::create("res/Null.png");
        dontClick->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
        dontClick->setScale(visibleSize.width, visibleSize.height);
        defeatScreen->addChild(dontClick);

        auto fadeBack = LayerColor::create(Color4B(0, 0, 0, 100),
            visibleSize.width, visibleSize.height);
        defeatScreen->addChild(fadeBack);

        this->addChild(defeatScreen, 0);
        defeatScreen->setVisible(false);
    }

    drawScreen = cocos2d::Sprite::create("res/Draw.png");
    drawScreen->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
    if (drawScreen != NULL)
    {
        auto dontClick = ui::Button::create("res/Null.png");
        dontClick->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
        dontClick->setScale(visibleSize.width, visibleSize.height);
        drawScreen->addChild(dontClick);

        auto fadeBack = LayerColor::create(Color4B(0, 0, 0, 100),
            visibleSize.width, visibleSize.height);
        drawScreen->addChild(fadeBack);

        this->addChild(drawScreen, 0);
        drawScreen->setVisible(false);
    }

    exitBnt = ui::Button::create("res/GameRoomExit_Normal.png", "res/GameRoomExit_Selected.png");
    if (exitBnt != NULL)
    {
        exitBnt->addClickEventListener(CC_CALLBACK_1(GameResultScreen::RoomExit, this));
        exitBnt->setAnchorPoint(Vec2(0.5, 0.5));
        float posX = (visibleSize.width ) / 2;
        float posY = (visibleSize.height) / 2 - 60;
        exitBnt->setPosition(Vec2(posX, posY));
        exitBnt->setScale(0.8, 0.8);

        this->addChild(exitBnt);
        exitBnt->setVisible(false);

    }

    return true;
}

void GameResultScreen::StartSchedule()
{
    this->schedule(CC_SCHEDULE_SELECTOR(GameResultScreen::UpdateInGame), 0.03f);
}

void GameResultScreen::Update(GameResult pGameresult)
{
    gameResult = pGameresult;
}

void GameResultScreen::UpdateInGame(float f)
{
    victoryScreen->setVisible(false);
    defeatScreen->setVisible(false);
    drawScreen->setVisible(false);

    switch (gameResult)
    {
        case GameResult::VICTORY:
        {
            victoryScreen->setVisible(true);
            exitBnt->setVisible(true);
        }
        break;
        case GameResult::DEFEAT:
        {
            defeatScreen->setVisible(true);
            exitBnt->setVisible(true);
        }
        break;
        case GameResult::DRAW:
        {
            drawScreen->setVisible(true);
            exitBnt->setVisible(true);
        }
        break;
        case GameResult::EMPTY:
        {
            exitBnt->setVisible(false);
        }
        break;
    }
}

void GameResultScreen::RoomExit(Ref* ref)
{
    Update(GameResult::EMPTY);
    UpdateInGame(0);
    SCENE_MGR.MoveScene("Room");
}
