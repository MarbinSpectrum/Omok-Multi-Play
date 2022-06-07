#include "Lobby.h"

USING_NS_CC;

const static Size roomSlotSize = Size(350, 100);
const static Size roomScrollViewSize = Size(roomSlotSize.width + 45, WINSIZEY - 55);

Scene* Lobby::createScene()
{
    return Lobby::create();
}

bool Lobby::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = DIRECTOR->getVisibleSize();
    Vec2 origin = DIRECTOR->getVisibleOrigin();

    //로비 배경 생성
    auto backGround = Sprite::create("res/LobbyBack.png");
    backGround->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(backGround, 0);

    //스크롤바 생성
    roomScrollView = ui::ScrollView::create();
    roomScrollView->setContentSize(Size(roomScrollViewSize.width, roomScrollViewSize.height));
    roomScrollView->setPosition(Point(75, 0));
    roomScrollView->setAnchorPoint(Vec2(0, 0));
    roomScrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
    roomScrollView->setScrollBarWidth(10);
    roomScrollView->setScrollBarPositionFromCorner(Vec2(2, 2));
    roomScrollView->setScrollBarColor(Color3B::WHITE); 
    
    for (int i = 0; i < 100; i++)
    {
        auto roomSlot = RoomSlot::create("",0, 0, 0);
        float posX = roomScrollView->getContentSize().width / 2;
        float posY = roomScrollView->getInnerContainerSize().height - roomSlotSize.height * i - roomSlotSize.height / 2;
        roomSlot->setPosition(Vec2(posX, posY));

        roomScrollView->addChild(roomSlot);

        roomSlotList.push_back(roomSlot);
    } 
    this->addChild(roomScrollView, 0);

    //새로고침 버튼
    auto refreshBtn = ui::Button::create("res/Refresh_Normal.png", "res/Refresh_Selected.png");
    if (refreshBtn != NULL)
    {
        float x = origin.x + 40;
        float y = origin.y + 290;
        refreshBtn->setPosition(Vec2(x, y));
        refreshBtn->addClickEventListener(CC_CALLBACK_1(Lobby::RoomListRefresh, this));
        this->addChild(refreshBtn, 1);
    }

    //방만들기 버튼
    auto lobbyRoomMakeBtn = ui::Button::create("res/LobbyRoomMake_Normal.png", "res/LobbyRoomMake_Selected.png");
    if (lobbyRoomMakeBtn != NULL)
    {
        float x = origin.x + 40;
        float y = origin.y + 170;
        lobbyRoomMakeBtn->setPosition(Vec2(x, y));
        lobbyRoomMakeBtn->addClickEventListener(CC_CALLBACK_1(Lobby::LobbyRoomMake, this));
        this->addChild(lobbyRoomMakeBtn);
    }

    //팝업생성
    roomEnterPopup = RoomEnterPopup::create();
    roomEnterPopup->setVisible(false);
    this->addChild(roomEnterPopup, 10);

    return true;
}

void Lobby::RequestLobbyRoomList()
{
    Message message(MessageType::LOBBY_ROOM_DATA_REQUEST);
    MESSAGE_MGR.SendMsg(message);
}

void Lobby::UpdateLobbyRoomList(RoomDataList* newRoomDataList)
{
    int roomCnt = newRoomDataList->size();

    roomScrollView->jumpToTop();
    roomScrollView->setInnerContainerSize(Size(roomScrollViewSize.width, roomSlotSize.height * roomCnt));
    for (int i = 0; i < 100; i++)
    {
        RoomSlot* roomSlot = roomSlotList[i];
        if (roomSlot == NULL)
        {
            return;
        }

        if (roomCnt <= i)
        {
            roomSlot->setVisible(false);
        }
        else
        {
            roomSlot->setVisible(true);

            RoomData& roomData = (*newRoomDataList)[i];
            std::string roomName = roomData.roomName;
            uint roomNum = roomData.roomNum;
            int64 roomKey = roomData.roomKey;
            int personNum = roomData.personNum;

            roomSlot->SetRoomName(roomName);
            roomSlot->SetRoomNum(roomNum);
            roomSlot->SetRoomKey(roomKey);
            roomSlot->SetPersonNum(personNum);

            float posX = roomScrollView->getContentSize().width / 2;
            float posY = roomScrollView->getInnerContainerSize().height - 
                roomSlotSize.height * i - roomSlotSize.height / 2;
            roomSlot->setPosition(Vec2(posX, posY));
        }
    }
}
void Lobby::Start()
{
    RequestLobbyRoomList();
    for (int i = 0; i < 100; i++)
    {
        RoomSlot* roomSlot = roomSlotList[i];
        if (roomSlot == NULL)
            continue;
        roomSlot->StartSchedule();
    }
}

void Lobby::LobbyRoomMake(Ref* pSender)
{
    Message message(MessageType::MAKE_ROOM_REQUEST);
    MESSAGE_MGR.SendMsg(message);
}

void Lobby::RoomListRefresh(Ref* pSender)
{
    RequestLobbyRoomList();
}

void Lobby::CantRoomEnter()
{
    roomEnterPopup->setVisible(true);
}

