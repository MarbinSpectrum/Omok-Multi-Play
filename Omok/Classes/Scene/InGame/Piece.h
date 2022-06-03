#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "framework.h"

USING_NS_CC;

enum class PieceType
{
    EMPTY,
    WHITE,
    BLACK
};

class Piece : public Label
{
public:
    Piece();
    ~Piece();

public:
    static  Piece*      create();

public:
    bool                init();
    void                Update(PieceType pPieceType);

public:
    Size                size;

private:
    cocos2d::Sprite*    white;
    cocos2d::Sprite*    black;
    ui::Button*         pieceBtn;

    PieceType           pieceType;
};