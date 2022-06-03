#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "framework.h"

USING_NS_CC;

class RoomSlot : public Label
{
public:
    RoomSlot();
    ~RoomSlot();

public:
    static  RoomSlot*   create(std::string roomName, uint roomNum, int64 roomKey, int roomPerson);

public:
    bool                init(std::string roomName, uint roomNum, int64 roomKey, int roomPerson);
    void                SetRoomName(std::string pRoomName);
    void                SetRoomNameText(std::string pRoomName);
    void                SetRoomNum(uint pRoomNum);
    void                SetRoomNumText(uint pRoomKey);
    void                SetPersonNum(int pRoomNum);
    void                SetPersonNumText(int pPersonNum);
    void                SetRoomKey(int64 pRoomKey);
    void                StartSchedule();

private:
    void                EnterRoom(Ref* pSender);
    void                UpdateUI(float f);

private:
    std::string         roomName;
    uint                roomNum;
    int64               roomKey;
    uint                personNum;

    Sprite*             slotBack;
    ui::Text*           roomNameText;
    ui::Text*           roomNumText;
    ui::Text*           personNumText;
};