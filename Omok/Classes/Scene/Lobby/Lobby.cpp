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

    auto visibleSize = DIRECTOR->getVisibleSize();
    Vec2 origin = DIRECTOR->getVisibleOrigin();

    //로비 배경 생성
    auto backGround = Sprite::create("res/LobbyBack.png");
    backGround->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(backGround, 0);

    //스크롤바 생성
    roomScrollView = ui::ScrollView::create();
    if (roomScrollView != NULL)
    {
        roomScrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
        roomScrollView->setContentSize(Size(roomScrollViewSize.width, roomScrollViewSize.height));
       
        roomScrollView->setInnerContainerSize(Size(roomScrollViewSize.width, roomSlotSize.height));
        //Sprite* roomSlot = Sprite::create("res/RoomSlot.png");
        roomScrollView->setAnchorPoint(Vec2(0, 0));
        float posX = roomScrollView->getContentSize().width / 2;
        float posY = roomScrollViewSize.height + roomSlotSize.height / 2;
        //roomSlot->setPosition(Vec2(posX, posY));
        //roomScrollView->addChild(roomSlot);
        

        roomScrollView->setBounceEnabled(false);
        roomScrollView->setPosition(Vec2(visibleSize.width - roomScrollViewSize.width + origin.x, origin.y));

        this->addChild(roomScrollView, 1);
    }

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

    return true;
}

void Lobby::RequestLobbyRoomList()
{
    Message message(MessageType::LOBBY_ROOM_DATA_REQUEST);
    std::string* nickName = PLAYER_MGR.GetNickName();

    message.WriteMessage(*nickName);
    MASSAGE_MGR.SendMsg(message);
}

void Lobby::UpdateLobbyRoomList(RoomDataList* newRoomDataList)
{
    int roomCnt = newRoomDataList->size();
    roomScrollView->setInnerContainerSize(Size(roomScrollViewSize.width, roomSlotSize.height * roomCnt));

    for (int i = 0; i < roomCnt; i++)
    {
        Sprite* roomSlot = Sprite::create("res/RoomSlot.png");
        float posX = roomScrollView->getContentSize().width / 2;
        float posY = roomSlotSize.height * i + roomSlotSize.height / 2;
        roomSlot->setPosition(Vec2(posX, posY));
        roomScrollView->addChild(roomSlot);
    }
}
void Lobby::Start()
{
    RequestLobbyRoomList();
}

void Lobby::LobbyRoomMake(Ref* pSender)
{
    Message message(MessageType::MAKE_ROOM_REQUEST);
    MASSAGE_MGR.SendMsg(message);
}

void Lobby::RoomListRefresh(Ref* pSender)
{
    RequestLobbyRoomList();
}

