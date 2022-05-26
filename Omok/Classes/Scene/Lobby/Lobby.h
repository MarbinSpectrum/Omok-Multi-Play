#pragma once

#include "ui/CocosGUI.h"
#include "cocos2d.h"
#include "framework.h"
#include "RoomData.h"
#include "RoomSlot.h"

class RoomData;
class RoomSlot;

#define CSCENE cocos2d::Scene
#define DIRECTOR Director::getInstance()

USING_NS_CC;

class Lobby : public CSCENE
{
public:
    typedef std::vector<RoomData> RoomDataList;

public:
    static CSCENE*  createScene();
    virtual bool    init();
    virtual void    Start() override;
    void            RequestLobbyRoomList();
    void            UpdateLobbyRoomList(RoomDataList* newRoomDataList);
    void            LobbyRoomMake(Ref* pSender);
    void            RoomListRefresh(Ref* pSender);

public:
    CREATE_FUNC(Lobby);

private:
    ui::ScrollView* roomScrollView;
    std::vector<RoomSlot*> roomSlotList;
};