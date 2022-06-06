#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "framework.h"
#include "PieceData.h"

USING_NS_CC;

class Piece : public Label
{
public:
    Piece(int pR,int pC);
    ~Piece();

public:
    static  Piece*      create(int pR, int pC);

public:
    bool                init();
    void                Update(PieceType pPieceType);
    void                StartSchedule();

public:
    Size                size;

private:
    void                UpdateInGame(float f);
    void                SetPiece(Ref* ref);

private:
    cocos2d::Sprite*    white;
    cocos2d::Sprite*    black;
    ui::Button*         pieceBtn;

    int                 r;
    int                 c;
    PieceType           pieceType;
};