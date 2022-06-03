#include "RoomEnterPopup.h"

USING_NS_CC;

RoomEnterPopup::RoomEnterPopup()
{
}

RoomEnterPopup::~RoomEnterPopup()
{
}

bool RoomEnterPopup::init()
{
    auto visibleSize = DIRECTOR->getVisibleSize();
    
    auto fadeBack = LayerColor::create(Color4B(0, 0, 0, 200),
        visibleSize.width, visibleSize.height);
    this->addChild(fadeBack);

    auto back = Sprite::create("res/NotRoomEnter.png");
    back->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(back);

    auto closePopup = ui::Button::create("res/Null.png");
    closePopup->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
    closePopup->setScale(visibleSize.width, visibleSize.height);
    closePopup->addClickEventListener(CC_CALLBACK_1(RoomEnterPopup::ClosePopUp, this));
    this->addChild(closePopup);

    return true;
}

void RoomEnterPopup::ClosePopUp(Ref* ref)
{
    this->setVisible(false);
}