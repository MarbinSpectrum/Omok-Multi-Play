#include "PlayerSlot.h"

USING_NS_CC;

PlayerSlot::PlayerSlot()
: backGround(NULL)
, playerNameText(NULL)
, playerReadyText(NULL)
, isHost(false)
, playerReady(false)
{
}

PlayerSlot::~PlayerSlot()
{
    this->unschedule(CC_SCHEDULE_SELECTOR(PlayerSlot::UpdateUI));
}

PlayerSlot* PlayerSlot::create(std::string pPlayerName, bool pisHost)
{
    PlayerSlot* ret = new PlayerSlot();
    if (ret && ret->init(pPlayerName, pisHost))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool PlayerSlot::init(std::string pPlayerName, bool pisHost)
{
    isHost = pisHost;

    backGround = Sprite::create("res/PlayerSlot.png");
    backGround->setAnchorPoint(Size(0.5, 0.5));
    this->addChild(backGround);

    //이름 객체 생성
    playerNameText = ui::Text::create(pPlayerName, "fonts/GodoM.ttf", 25);
    playerNameText->setTextColor(Color4B(0, 0, 0, 255));
    backGround->addChild(playerNameText);

    //준비상태 객체 생성
    playerReadyText = ui::Text::create("", "fonts/GodoM.ttf", 25);
    playerReadyText->setTextColor(Color4B(0, 255, 0, 255));
    backGround->addChild(playerReadyText);

    slotSize = backGround->getContentSize();
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : 플레이어 텍스트 등록
////////////////////////////////////////////////////////////////////////////////////////////////
void PlayerSlot::SetText(std::string pPlayerName)
{
    playerName = pPlayerName;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : 플레이어 준비상태 등록
////////////////////////////////////////////////////////////////////////////////////////////////
void PlayerSlot::SetReady(bool pPlayerReady)
{
    playerReady = pPlayerReady;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : 스케줄시작
////////////////////////////////////////////////////////////////////////////////////////////////
void PlayerSlot::StartSchedule()
{
    this->schedule(CC_SCHEDULE_SELECTOR(PlayerSlot::UpdateUI), 0.03f);
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : UI 주기적 갱신
////////////////////////////////////////////////////////////////////////////////////////////////
void PlayerSlot::UpdateUI(float f)
{
    if (playerNameText != NULL)
    {
        if (playerName.size() > 0)
        {
            playerNameText->setString(playerName);
        }
        else
        {
            playerNameText->setString("Empty");
        }

        float posX = backGround->getContentSize().width - (playerNameText->getContentSize().width / 2 + 10);
        float posY = backGround->getContentSize().height - (playerNameText->getContentSize().height / 2 + 10);
        playerNameText->setPosition(Vec2(posX, posY));
    }

    if (playerReadyText != NULL)
    {
        if (isHost)
        {
            playerReadyText->setString("★");
            playerReadyText->enableOutline(Color4B(0, 0, 0, 255), 1);
            playerReadyText->setTextColor(Color4B(255, 255, 0, 255));
            float posX = playerReadyText->getContentSize().width / 2 + 10;
            float posY = backGround->getContentSize().height - (playerReadyText->getContentSize().height / 2 + 10);
            playerReadyText->setPosition(Vec2(posX, posY));
        }
        else if(playerReady)
        {
            playerReadyText->setString("Ready");
            playerReadyText->enableOutline(Color4B(0, 0, 0, 255), 0);
            playerReadyText->setTextColor(Color4B(0, 255, 0, 255));
            float posX = playerReadyText->getContentSize().width / 2 + 10;
            float posY = playerReadyText->getContentSize().height / 2 + 10;
            playerReadyText->setPosition(Vec2(posX, posY));
        }
        else
        {
            playerReadyText->setString("");
        }
    }
}