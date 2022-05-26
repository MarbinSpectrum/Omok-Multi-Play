#include "RoomSlot.h"

USING_NS_CC;

RoomSlot::RoomSlot()
: slotBack(NULL)
, roomNumText(NULL)
, personNumText(NULL)
{
}

RoomSlot::~RoomSlot()
{
}

RoomSlot* RoomSlot::create(uint roomNum, int64 roomKey, int roomPerson)
{
    RoomSlot* ret = new RoomSlot();
    if (ret && ret->init(roomNum, roomKey, roomPerson))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool RoomSlot::init(uint roomNum, int64 roomKey, int roomPerson)
{
    slotBack = Sprite::create("res/RoomSlot.png");

    //规 锅龋 按眉 积己
    roomNumText = ui::Text::create("NO.1234567890/", "fonts/GodoM.ttf", 35);
    if (roomNumText != NULL)
    {
        SetRoomNumText(roomNum);
        roomNumText->setTextColor(Color4B(0, 0, 0, 255));
        slotBack->addChild(roomNumText);
    }

    //规 牢盔荐 按眉 积己
    personNumText = ui::Text::create("1234567890/", "fonts/GodoM.ttf", 30);
    if (personNumText != NULL)
    {
        SetPersonNumText(roomPerson);
        personNumText->setTextColor(Color4B(0, 0, 0, 255));
        slotBack->addChild(personNumText);
    }

    auto enterBtn = ui::Button::create("res/RoomEnter_Normal.png", "res/RoomEnter_Selected.png");
    if (enterBtn != NULL)
    {
        enterBtn->addClickEventListener(CC_CALLBACK_1(RoomSlot::EnterRoom, this));
        float labelPosX = enterBtn->getContentSize().width / 2 + 10;
        float labelPosY = enterBtn->getContentSize().height / 2 + 10;
        enterBtn->setPosition(Vec2(labelPosX, labelPosY));
        slotBack->addChild(enterBtn);
    }

    this->addChild(slotBack);

    return true;
}

void RoomSlot::EnterRoom(Ref* pSender)
{
    Message message(MessageType::ENTER_ROOM_REQUEST);
    message.WriteMessage(std::to_string(roomNum));
    message.WriteMessage(std::to_string(roomKey));

    MASSAGE_MGR.SendMsg(message);
}

void RoomSlot::SetRoomNum(uint pRoomNum)
{
    roomNum = pRoomNum;
    SetRoomNumText(pRoomNum);
}

void RoomSlot::SetRoomKey(int64 pRoomKey)
{
    roomKey = pRoomKey;
}

void RoomSlot::SetPersonNum(int pPersonNum)
{
    SetPersonNumText(pPersonNum);
}

void RoomSlot::SetRoomNumText(uint roomNum)
{
    roomNumText->setString("NO." + std::to_string(roomNum));
    float labelPosX = roomNumText->getContentSize().width / 2 + 10;
    float labelPosY = slotBack->getContentSize().height - (roomNumText->getContentSize().height / 2 + 10);
    roomNumText->setPosition(Vec2(labelPosX, labelPosY));
}

void RoomSlot::SetPersonNumText(int roomPerson)
{
    personNumText->setString(std::to_string(roomPerson) + "/2");
    float labelPosX = slotBack->getContentSize().width - (personNumText->getContentSize().width / 2 + 10);
    float labelPosY = personNumText->getContentSize().height / 2 + 10;
    personNumText->setPosition(Vec2(labelPosX, labelPosY));
}
