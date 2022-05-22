#include "Lobby.h"

USING_NS_CC;

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

    return true;
}

