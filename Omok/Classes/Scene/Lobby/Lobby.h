#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "framework.h"
#include "RoomData/RoomData.h"
#include <vector>

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
    void RequestLobbyRoomList();
    void UpdateLobbyRoomList(RoomDataList* newRoomDataList);

public:
    CREATE_FUNC(Lobby);

private:
    ui::ScrollView* roomScrollView;
    RoomDataList roomDataList;
};