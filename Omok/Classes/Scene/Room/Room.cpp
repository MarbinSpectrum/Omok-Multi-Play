#include "Room.h"

USING_NS_CC;

Scene* Room::createScene()
{
    return Room::create();
}

bool Room::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = DIRECTOR->getVisibleSize();
    Vec2 origin = DIRECTOR->getVisibleOrigin();

    //배경 생성
    auto backGround = Sprite::create("res/GameRoomBack.png");
    backGround->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(backGround, 0);

    //호스트 슬롯
    hostSlot = PlayerSlot::create("",true);
    if (hostSlot != NULL)
    {
        hostSlot->setAnchorPoint(Vec2(0.5, 0.5));
        float posX = hostSlot->slotSize.width/2 + 10;
        float posY = visibleSize.height - (hostSlot->slotSize.height/2 + 100);
        hostSlot->setPosition(Vec2(posX, posY));
        this->addChild(hostSlot, 0);
    }

    //플레이어 슬롯
    guestSlot = PlayerSlot::create("", false);
    if (guestSlot != NULL)
    {
        guestSlot->setAnchorPoint(Vec2(0.5, 0.5));
        float posX = visibleSize.width - (guestSlot->slotSize.width/2 + 10);
        float posY = visibleSize.height - (guestSlot->slotSize.height/2 + 100);
        guestSlot->setPosition(Vec2(posX, posY));
        this->addChild(guestSlot, 0);
    }

    roomBtn = RoomBtn::create();
    if (roomBtn != NULL)
    {
        float posX = roomBtn->btnSize.width / 2 + 10;
        float posY = roomBtn->btnSize.height / 2 + 10;
        roomBtn->setPosition(Vec2(posX, posY));
        this->addChild(roomBtn);
    }

    auto exitBnt = ui::Button::create("res/GameRoomExit_Normal.png", "res/GameRoomExit_Selected.png");
    if (exitBnt != NULL)
    {
        exitBnt->addClickEventListener(CC_CALLBACK_1(Room::RoomExit, this));
        float posX = visibleSize.width - (exitBnt->getContentSize().width / 2 + 10);
        float posY = exitBnt->getContentSize().height / 2 + 10;
        exitBnt->setPosition(Vec2(posX, posY));
        this->addChild(exitBnt);
    }
    return true;
}

void Room::Start()
{
    Message message(MessageType::GAMEROOM_DATA_REQUEST);
    MASSAGE_MGR.SendMsg(message);

    hostSlot->StartSchedule();
    guestSlot->StartSchedule();
    roomBtn->StartSchedule();
}

void Room::UpdateRoom(std::string pHost, std::string pGuest, bool pReady, bool isHost)
{
    if (hostSlot == NULL)
        return;
    if (guestSlot == NULL)
        return;
    if (guestSlot == NULL)
        return;

    hostSlot->SetText(pHost);
    guestSlot->SetText(pGuest);
    guestSlot->SetReady(pReady);

    if (isHost)
    {
        if (pReady)
        {
            roomBtn->SetBtnType(RoomBtnType::GAME_START_BTN);
        }
        else
        {
            roomBtn->SetBtnType(RoomBtnType::GAME_START_NOT_ACT_BTN);
        }
    }
    else
    {
        if (pReady)
        {
            roomBtn->SetBtnType(RoomBtnType::GAME_READY_OK_BTN);
        }
        else
        {
            roomBtn->SetBtnType(RoomBtnType::GAME_READY_BTN);
        }
    }
}

void Room::RoomExit(Ref* ref)
{
    Message message(MessageType::EXIT_ROOM_REQUEST);
    MASSAGE_MGR.SendMsg(message);
}
