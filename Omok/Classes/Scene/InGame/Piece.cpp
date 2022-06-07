#include "Piece.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;

Piece::Piece(int pR, int pC)
: white(NULL)
, black(NULL)
, pieceBtn(NULL)
, pieceType(PieceType::EMPTY)
, r(pR)
, c(pC)
{
}

Piece::~Piece()
{
    this->unschedule(CC_SCHEDULE_SELECTOR(Piece::UpdateInGame));
}

Piece* Piece::create(int pR,int pC)
{
    Piece* ret = new Piece(pR, pC);
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

bool Piece::init()
{
    white = Sprite::create("res/White.png");
    white->setAnchorPoint(Size(0.5, 0.5));
    this->addChild(white);

    black = Sprite::create("res/Black.png");
    black->setAnchorPoint(Size(0.5, 0.5));
    this->addChild(black);

    pieceBtn = ui::Button::create("res/White.png");
    pieceBtn->setOpacity(0);
    pieceBtn->setAnchorPoint(Size(0.5, 0.5));
    pieceBtn->addClickEventListener(CC_CALLBACK_1(Piece::SetPiece, this));
    this->addChild(pieceBtn);

    size = white->getContentSize();

    return true;
}

void Piece::Update(PieceType pPieceType)
{
    pieceType = pPieceType;
}

void Piece::StartSchedule()
{
    this->schedule(CC_SCHEDULE_SELECTOR(Piece::UpdateInGame), 0.03f);
}

void Piece::UpdateInGame(float f)
{
    white->setVisible(false);
    black->setVisible(false);
    pieceBtn->setVisible(false);

    switch (pieceType)
    {
    case PieceType::EMPTY:
        pieceBtn->setVisible(true);
        break;
    case PieceType::WHITE:
        white->setVisible(true);
        break;
    case PieceType::BLACK:
        black->setVisible(true);
        break;
    }
}

void Piece::SetPiece(Ref* ref)
{
    Message message(MessageType::GAMEBOARD_SET_PIECE);
    message.WriteMessage(to_string(r));
    message.WriteMessage(to_string(c));

    MESSAGE_MGR.SendMsg(message);
}
