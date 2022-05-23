#include "Lobby.h"

USING_NS_CC;

const static Size roomSlotSize = Size(350, 100);
const static Size roomScrollViewSize = Size(roomSlotSize.width + 65, WINSIZEY - 55);

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
    
    RequestLobbyRoomList();

    auto visibleSize = DIRECTOR->getVisibleSize();
    Vec2 origin = DIRECTOR->getVisibleOrigin();

    //로비 배경 생성
    auto backGround = Sprite::create("res/LobbyBack.png");
    backGround->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(backGround, 0);

    roomScrollView = ui::ScrollView::create();
    if (roomScrollView != NULL)
    {
        roomScrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
        roomScrollView->setContentSize(Size(roomScrollViewSize.width, roomScrollViewSize.height));
        //roomScrollView->setInnerContainerSize(Size(ss.width, roomSlotSize.height * 50));

        roomScrollView->setBounceEnabled(false);
        roomScrollView->setPosition(Vec2(visibleSize.width - roomScrollViewSize.width + origin.x, origin.y));

        for (int i = 0; i < 50; i++)
        {
            Sprite* roomSlot = Sprite::create("res/RoomSlot.png");
            roomSlot->setPosition(Vec2(roomScrollView->getContentSize().width / 2, roomSlotSize.height / 2 + roomSlotSize.height * i));
            roomScrollView->addChild(roomSlot);
        }

        this->addChild(roomScrollView);
    }

    return true;
}

void Lobby::RequestLobbyRoomList()
{
    Message message(MessageType::LOBBY_ROOM_DATA_REQUEST);
    MASSAGE_MGR.SendMsg(message);
}


void Lobby::UpdateLobbyRoomList(RoomDataList* newRoomDataList)
{
    roomDataList.clear();


}

