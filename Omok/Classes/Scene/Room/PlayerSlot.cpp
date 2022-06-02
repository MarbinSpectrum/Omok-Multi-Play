#include "PlayerSlot.h"

USING_NS_CC;

PlayerSlot::PlayerSlot()
{
}

PlayerSlot::~PlayerSlot()
{
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
    this->schedule(CC_SCHEDULE_SELECTOR(PlayerSlot::UpdateUI), 0.1f);
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

        float labelPosX = backGround->getContentSize().width - (playerNameText->getContentSize().width / 2 + 10);
        float labelPosY = backGround->getContentSize().height - (playerNameText->getContentSize().height / 2 + 10);
        playerNameText->setPosition(Vec2(labelPosX, labelPosY));
    }

    if (playerReadyText != NULL)
    {
        if (isHost)
        {
            playerReadyText->setString("★");
        }
        else if(playerReady)
        {
            playerReadyText->setString("Ready");
        }
        else
        {
            playerReadyText->setString("");
        }

        float labelPosX = playerReadyText->getContentSize().width / 2 + 10;
        float labelPosY = playerReadyText->getContentSize().height / 2 + 10;
        playerReadyText->setPosition(Vec2(labelPosX, labelPosY));
    }
}