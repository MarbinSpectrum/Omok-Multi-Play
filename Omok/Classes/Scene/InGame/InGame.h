#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "framework.h"
#include "Piece.h"
#include "PieceData.h"
#include "DontClick.h"

class Message;
class Piece;
class DontClick;

#define CSCENE cocos2d::Scene
#define DIRECTOR Director::getInstance()

USING_NS_CC;

class InGame : public CSCENE
{
    typedef  unordered_map<int, unordered_map<int, Piece*>> GameBoardMap;
    typedef  vector<PieceData> PieceDatas;

public:
    InGame();
    ~InGame();

    static CSCENE*  createScene();

    virtual bool    init();
    virtual void    Start() override;
    void            UpdateGameBoard(Message& message);

public:
    CREATE_FUNC(InGame);

private:
    GameBoardMap*   piece;
    PieceDatas*     pieceData;
    DontClick*      dontClick;

    const int       boardR = 19;
    const int       boardC = 19;
    bool            yourTurn;
};