#include "LoginPopup.h"

USING_NS_CC;

LoginPopup::LoginPopup()
{
}

LoginPopup::~LoginPopup()
{
}

bool LoginPopup::init()
{
    auto visibleSize = DIRECTOR->getVisibleSize();
    
    auto fadeBack = LayerColor::create(Color4B(0, 0, 0, 200),
        visibleSize.width, visibleSize.height);
    this->addChild(fadeBack);

    auto back = Sprite::create("res/NotLogin.png");
    back->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(back);

    return true;
}