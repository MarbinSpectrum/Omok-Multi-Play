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

    //��� �ؽ�Ʈ ���
    auto labelText = Label::createWithTTF("�г����� �Է����ּ���!!", "fonts/GodoM.ttf", 18);
    if (labelText != NULL)
    {
        labelText->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height - labelText->getContentSize().height - 70));
        this->addChild(labelText, 1);
    }

    //�ؽ�Ʈ �Է¹�
    textField = ui::TextField::create("(8�� �̸�)", "fonts/GodoM.ttf", 12);

    if (textField != NULL)
    {
        textField->setMaxLengthEnabled(true);
        textField->setMaxLength(8);
        textField->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height - textField->getContentSize().height - 120));
        this->addChild(textField, 1);
    }
  
    //Ȯ�ι�ư ����
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
    Message message(MessageType::LOBBY_ENTER_REQUEST);
    
    std::string nickName = textField->getString();
    message.WriteMessage(nickName);


    MASSAGE_MGR.SendMsg(message);
}
