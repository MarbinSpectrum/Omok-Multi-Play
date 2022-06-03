#include "RoomSlot.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;

using namespace cocostudio::timeline;

RoomSlot::RoomSlot()
: slotBack(NULL)
, roomNumText(NULL)
, personNumText(NULL)
{
}

RoomSlot::~RoomSlot()
{
    this->unschedule(CC_SCHEDULE_SELECTOR(RoomSlot::UpdateUI));
}

RoomSlot* RoomSlot::create(std::string roomName, uint roomNum, int64 roomKey, int roomPerson)
{
    RoomSlot* ret = new RoomSlot();
    if (ret && ret->init(roomName, roomNum, roomKey, roomPerson))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : 방 초기화
////////////////////////////////////////////////////////////////////////////////////////////////
bool RoomSlot::init(std::string roomName, uint roomNum, int64 roomKey, int roomPerson)
{
    slotBack = Sprite::create("res/RoomSlot.png");
    if (slotBack != NULL)
    {
        this->addChild(slotBack);
    }

    //방 이름 객체 생성
    roomNameText = ui::Text::create("", "fonts/GodoM.ttf", 35);
    if (roomNameText != NULL)
    {
        SetRoomNumText(roomNum);
        roomNameText->setTextColor(Color4B(0, 0, 0, 255));
        slotBack->addChild(roomNameText);
    }

    //방 번호 객체 생성
    roomNumText = ui::Text::create("", "fonts/GodoM.ttf", 35);
    if (roomNumText != NULL)
    {
        SetRoomNumText(roomNum);
        roomNumText->setTextColor(Color4B(0, 0, 0, 255));
        slotBack->addChild(roomNumText);
    }

    //방 인원수 객체 생성
    personNumText = ui::Text::create("", "fonts/GodoM.ttf", 30);
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
        float posX = enterBtn->getContentSize().width / 2 + 10;
        float posY = enterBtn->getContentSize().height / 2 + 10;
        enterBtn->setPosition(Vec2(posX, posY));
        slotBack->addChild(enterBtn);
    }

    this->schedule(CC_SCHEDULE_SELECTOR(RoomSlot::UpdateUI), 0.1f);

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : 방 이름
////////////////////////////////////////////////////////////////////////////////////////////////
void RoomSlot::SetRoomName(std::string pRoomName)
{
    roomName = pRoomName;
}
void RoomSlot::SetRoomNameText(std::string pRoomName)
{
    if (roomNameText == NULL)
        return;
    roomNameText->setString(pRoomName + "의 방");
    float posX = slotBack->getContentSize().width - (roomNameText->getContentSize().width / 2 + 10);
    float posY = slotBack->getContentSize().height - (roomNameText->getContentSize().height / 2 + 10);
    roomNameText->setPosition(Vec2(posX, posY));
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : 방 번호
////////////////////////////////////////////////////////////////////////////////////////////////
void RoomSlot::SetRoomNum(uint pRoomNum)
{
    roomNum = pRoomNum;
}
void RoomSlot::SetRoomNumText(uint roomNum)
{
    if (roomNumText == NULL)
        return;
    roomNumText->setString("NO." + std::to_string(roomNum));
    float posX = roomNumText->getContentSize().width / 2 + 10;
    float posY = slotBack->getContentSize().height - (roomNumText->getContentSize().height / 2 + 10);
    roomNumText->setPosition(Vec2(posX, posY));
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : 방 인원
////////////////////////////////////////////////////////////////////////////////////////////////
void RoomSlot::SetPersonNum(int pPersonNum)
{
    personNum = pPersonNum;
}
void RoomSlot::SetPersonNumText(int roomPerson)
{
    if (personNumText == NULL)
        return;
    int maxPerson = RoomData::maxPerson;
    personNumText->setString(std::to_string(roomPerson) + "/" + std::to_string(maxPerson));
    float posX = slotBack->getContentSize().width - (personNumText->getContentSize().width / 2 + 10);
    float posY = personNumText->getContentSize().height / 2 + 10;
    personNumText->setPosition(Vec2(posX, posY));
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : 방의 키값
////////////////////////////////////////////////////////////////////////////////////////////////
void RoomSlot::SetRoomKey(int64 pRoomKey)
{
    roomKey = pRoomKey;
}


////////////////////////////////////////////////////////////////////////////////////////////////
/// : 방 입장
////////////////////////////////////////////////////////////////////////////////////////////////
void RoomSlot::EnterRoom(Ref* pSender)
{
    Message message(MessageType::ENTER_ROOM_REQUEST);
    message.WriteMessage(std::to_string(roomNum));
    message.WriteMessage(std::to_string(roomKey));

    MASSAGE_MGR.SendMsg(message);
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : 스케줄시작
////////////////////////////////////////////////////////////////////////////////////////////////
void RoomSlot::StartSchedule()
{
    this->schedule(CC_SCHEDULE_SELECTOR(RoomSlot::UpdateUI), 0.03f);
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : UI 주기적 갱신
////////////////////////////////////////////////////////////////////////////////////////////////
void RoomSlot::UpdateUI(float f)
{
    SetRoomNameText(roomName);
    SetRoomNumText(roomNum);
    SetPersonNumText(personNum);
}