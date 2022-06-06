#include "DontClick.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;

DontClick::DontClick()
: dontClick(NULL)
, state(false)
{
}

DontClick::~DontClick()
{
    this->unschedule(CC_SCHEDULE_SELECTOR(DontClick::UpdateInGame));
}

DontClick* DontClick::create()
{
    DontClick* ret = new DontClick();
    if (ret && ret->init())
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool DontClick::init()
{
    auto visibleSize = DIRECTOR->getVisibleSize();
    Vec2 origin = DIRECTOR->getVisibleOrigin();

    dontClick = ui::Button::create("res/Null.png");
    dontClick->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
    dontClick->setScale(visibleSize.width, visibleSize.height);
    auto fadeBack = LayerColor::create(Color4B(0, 0, 0, 100),
        visibleSize.width, visibleSize.height);
    dontClick->addChild(fadeBack);

    this->addChild(dontClick, 0);

    return true;
}

void DontClick::StartSchedule()
{
    this->schedule(CC_SCHEDULE_SELECTOR(DontClick::UpdateInGame), 0.03f);
}

void DontClick::Update(bool pState)
{
    state = pState;
}

void DontClick::UpdateInGame(float f)
{
    dontClick->setVisible(state);
}
