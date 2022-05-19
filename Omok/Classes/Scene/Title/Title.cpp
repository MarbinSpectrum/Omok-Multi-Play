#include "Title.h"

USING_NS_CC;

Scene* Title::createScene()
{
    return Title::create();
}

bool Title::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = DIRECTOR->getVisibleSize();
    Vec2 origin = DIRECTOR->getVisibleOrigin();

    //헤더 텍스트 출력
    auto labelText = Label::createWithTTF("닉네임을 입력해주세요!!", "fonts/GodoM.ttf", 18);
    if (labelText != NULL)
    {
        labelText->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height - labelText->getContentSize().height - 70));

        // add the label as a child to this layer
        this->addChild(labelText, 1);
    }

    auto textField = ui::TextField::create("(8자 미만)", "fonts/GodoM.ttf", 12);
    if (textField != NULL)
    {
        textField->setMaxLengthEnabled(true);
        textField->setMaxLength(8);
        textField->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height - textField->getContentSize().height - 120));

        // add the label as a child to this layer
        this->addChild(textField, 1);
    }
  

    //확인버튼 생성
    auto enterGameLobbyBtn = MenuItemImage::create(
        "res/OK_Normal.png", "res/OK_Selected.png",
        CC_CALLBACK_1(Title::EnterGameLobby, this));

    if (enterGameLobbyBtn != NULL)
    {
        float x = origin.x + visibleSize.width * 0.5f;
        float y = origin.y + visibleSize.height * 0.4f;
        enterGameLobbyBtn->setPosition(Vec2(x, y));

        auto menu = Menu::create(enterGameLobbyBtn, NULL);
        menu->setPosition(Vec2::ZERO);
        this->addChild(menu, 1);
    }
    return true;
}


void Title::EnterGameLobby(Ref* pSender)
{
    
}
