#include "RoomBtn.h"

USING_NS_CC;

RoomBtn::RoomBtn()
{
}

RoomBtn::~RoomBtn()
{
}

RoomBtn* RoomBtn::create()
{
    RoomBtn* ret = new RoomBtn();
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

bool RoomBtn::init()
{
    readyBtn = ui::Button::create("res/GameRoomReady_Normal.png", "res/GameRoomReady_Selected.png");
    if (readyBtn != NULL)
    {
        readyBtn->addClickEventListener(CC_CALLBACK_1(RoomBtn::ReadyBtn, this));
        this->addChild(readyBtn);
    }

    readyOKBtn = ui::Button::create("res/GameRoomReadyOK_Normal.png", "res/GameRoomReadyOK_Selected.png");
    if (readyOKBtn != NULL)
    {
        readyOKBtn->addClickEventListener(CC_CALLBACK_1(RoomBtn::ReadyOKBtn, this));
        this->addChild(readyOKBtn);
    }

    startBtn = ui::Button::create("res/GameRoomStart_Normal.png", "res/GameRoomStart_Selected.png");
    if (startBtn != NULL)
    {
        startBtn->addClickEventListener(CC_CALLBACK_1(RoomBtn::GameStartBtn, this));
        this->addChild(startBtn);
    }

    startBtnNotAct = cocos2d::Sprite::create("res/GameRoomStart_Normal.png");
    if (startBtnNotAct != NULL)
    {
        startBtnNotAct->setOpacity(125);
        this->addChild(startBtnNotAct);
        btnSize = startBtnNotAct->getContentSize();
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : 버튼 상태 설정
////////////////////////////////////////////////////////////////////////////////////////////////
void RoomBtn::SetBtnType(RoomBtnType pRoomBtnType)
{
    roomBtnType = pRoomBtnType;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : 스케줄시작
////////////////////////////////////////////////////////////////////////////////////////////////
void RoomBtn::StartSchedule()
{
    this->schedule(CC_SCHEDULE_SELECTOR(RoomBtn::UpdateUI), 0.03f);
}

void RoomBtn::UpdateUI(float f)
{
    readyBtn->setVisible(false);
    readyOKBtn->setVisible(false);
    startBtn->setVisible(false);
    startBtnNotAct->setVisible(false);

    switch (roomBtnType)
    {
        case RoomBtnType::GAME_READY_BTN:
        {
            readyBtn->setVisible(true);
        }
        break;
        case RoomBtnType::GAME_READY_OK_BTN:
        {
            readyOKBtn->setVisible(true);
        }
        break;
        case RoomBtnType::GAME_START_BTN:
        {
            startBtn->setVisible(true);
        }
        break;
        case RoomBtnType::GAME_START_NOT_ACT_BTN:
        {
            startBtnNotAct->setVisible(true);
        }
        break;
    }
}

void RoomBtn::ReadyBtn(Ref* ref)
{
    Message message(MessageType::GAMEROOM_READY_UPDATE);
    message.WriteMessage(true);

    MASSAGE_MGR.SendMsg(message);
}

void RoomBtn::ReadyOKBtn(Ref* ref)
{
    Message message(MessageType::GAMEROOM_READY_UPDATE);
    message.WriteMessage(false);

    MASSAGE_MGR.SendMsg(message);
}

void RoomBtn::GameStartBtn(Ref* ref)
{
}
