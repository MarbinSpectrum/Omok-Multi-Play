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
/// : �� �ʱ�ȭ
////////////////////////////////////////////////////////////////////////////////////////////////
bool RoomSlot::init(std::string roomName, uint roomNum, int64 roomKey, int roomPerson)
{
    slotBack = Sprite::create("res/RoomSlot.png");
    if (slotBack != NULL)
    {
        this->addChild(slotBack);
    }

    //�� �̸� ��ü ����
    roomNameText = ui::Text::create("", "fonts/GodoM.ttf", 35);
    if (roomNameText != NULL)
    {
        SetRoomNumText(roomNum);
        roomNameText->setTextColor(Color4B(0, 0, 0, 255));
        slotBack->addChild(roomNameText);
    }

    //�� ��ȣ ��ü ����
    roomNumText = ui::Text::create("", "fonts/GodoM.ttf", 35);
    if (roomNumText != NULL)
    {
        SetRoomNumText(roomNum);
        roomNumText->setTextColor(Color4B(0, 0, 0, 255));
        slotBack->addChild(roomNumText);
    }

    //�� �ο��� ��ü ����
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
/// : �� �̸�
////////////////////////////////////////////////////////////////////////////////////////////////
void RoomSlot::SetRoomName(std::string pRoomName)
{
    roomName = pRoomName;
}
void RoomSlot::SetRoomNameText(std::string pRoomName)
{
    if (roomNameText == NULL)
        return;
    roomNameText->setString(pRoomName + "�� ��");
    float posX = slotBack->getContentSize().width - (roomNameText->getContentSize().width / 2 + 10);
    float posY = slotBack->getContentSize().height - (roomNameText->getContentSize().height / 2 + 10);
    roomNameText->setPosition(Vec2(posX, posY));
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : �� ��ȣ
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
/// : �� �ο�
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
/// : ���� Ű��
////////////////////////////////////////////////////////////////////////////////////////////////
void RoomSlot::SetRoomKey(int64 pRoomKey)
{
    roomKey = pRoomKey;
}


////////////////////////////////////////////////////////////////////////////////////////////////
/// : �� ����
////////////////////////////////////////////////////////////////////////////////////////////////
void RoomSlot::EnterRoom(Ref* pSender)
{
    Message message(MessageType::ENTER_ROOM_REQUEST);
    message.WriteMessage(std::to_string(roomNum));
    message.WriteMessage(std::to_string(roomKey));

    MASSAGE_MGR.SendMsg(message);
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : �����ٽ���
////////////////////////////////////////////////////////////////////////////////////////////////
void RoomSlot::StartSchedule()
{
    this->schedule(CC_SCHEDULE_SELECTOR(RoomSlot::UpdateUI), 0.03f);
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : UI �ֱ��� ����
////////////////////////////////////////////////////////////////////////////////////////////////
void RoomSlot::UpdateUI(float f)
{
    SetRoomNameText(roomName);
    SetRoomNumText(roomNum);
    SetPersonNumText(personNum);
}