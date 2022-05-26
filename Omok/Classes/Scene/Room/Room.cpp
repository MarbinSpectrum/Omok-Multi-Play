#include "Room.h"

USING_NS_CC;

Scene* Room::createScene()
{
    return Room::create();
}

bool Room::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = DIRECTOR->getVisibleSize();
    Vec2 origin = DIRECTOR->getVisibleOrigin();

    playerName0 = ui::Text::create("", "fonts/GodoM.ttf", 30);
    playerName1 = ui::Text::create("", "fonts/GodoM.ttf", 30);

    return true;
}
void Room::Start()
{
    Message message(MessageType::ROOM_IS_HOST_REQUEST);
    MASSAGE_MGR.SendMsg(message);
}