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
    return true;
}

void Room::Start()
{
    Message message(MessageType::GAMEROOM_DATA_REQUEST);
    MASSAGE_MGR.SendMsg(message);
}

void Room::UpdateRoom(std::string pHost, std::string pGuest, bool pReady)
{
    hostSlot->SetText(pHost);
    guestSlot->SetText(pGuest);
    guestSlot->SetReady(pReady);
}
