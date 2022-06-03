#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "framework.h"
#include "Piece.h"

class Piece;

#define CSCENE cocos2d::Scene
#define DIRECTOR Director::getInstance()

USING_NS_CC;

class InGame : public CSCENE
{
    typedef  unordered_map<int, unordered_map<int, Piece*>> GameBoardMap;
    typedef  vector<pair<pair<int, int>, PieceType>> PieceDatas;
public:
    static CSCENE*  createScene();
    virtual bool    init();
    virtual void    Start() override;
    void            UpdateGameBoard(PieceDatas& pPieceData);

private:
    void            UpdateInGame(float f);

public:
    CREATE_FUNC(InGame);

private:
    const int boardR = 19;
    const int boardC = 19;
    GameBoardMap piece;
    PieceDatas pieceData;
};