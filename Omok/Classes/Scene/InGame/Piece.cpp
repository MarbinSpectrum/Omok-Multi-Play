#include "Piece.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;

Piece::Piece()
: white(NULL)
, black(NULL)
, pieceBtn(NULL)
, pieceType(PieceType::EMPTY)
{
}

Piece::~Piece()
{
}

Piece* Piece::create()
{
    Piece* ret = new Piece();
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

    black = Sprite::create("res/White.png");
    black->setAnchorPoint(Size(0.5, 0.5));
    this->addChild(black);

    pieceBtn = ui::Button::create("res/White.png");
    pieceBtn->setOpacity(0);
    pieceBtn->setAnchorPoint(Size(0.5, 0.5));
    this->addChild(pieceBtn);

    size = white->getContentSize();

    Update(pieceType);

    return true;
}

void Piece::Update(PieceType pPieceType)
{
    pieceType = pPieceType;
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
