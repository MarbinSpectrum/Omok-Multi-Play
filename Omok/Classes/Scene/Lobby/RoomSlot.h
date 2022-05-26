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
    static  RoomSlot*   create(uint roomNum, int64 roomKey, int roomPerson);

public:
    bool                init(uint roomNum, int64 roomKey, int roomPerson);
    void                SetRoomNum(uint pRoomNum);
    void                SetRoomNumText(uint pRoomKey);
    void                SetPersonNum(int pRoomNum);
    void                SetPersonNumText(int pPersonNum);
    void                SetRoomKey(int64 pRoomKey);

private:
    void                EnterRoom(Ref* pSender);

private:
    int                 roomNum;
    int                 roomKey;
    Sprite*             slotBack;
    ui::Text*           roomNumText;
    ui::Text*           personNumText;
};