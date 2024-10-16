﻿#include "InGame.h"
#include "../Classes/Manager/MessageMgr/MessageMgr.h"

USING_NS_CC;

Scene* InGame::createScene()
{
    return InGame::create();
}

InGame::InGame()
: piece(new GameBoardMap)
, pieceData(new PieceDatas)
{
}

InGame::~InGame()
{
    delete piece;
    piece = NULL;
    delete pieceData;
    pieceData = NULL;
    
}

bool InGame::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = DIRECTOR->getVisibleSize();
    Vec2 origin = DIRECTOR->getVisibleOrigin();

    auto gameBoard = Sprite::create("res/GameBoard.png");
    gameBoard->setAnchorPoint(Vec2(0.5, 0.5));
    gameBoard->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    gameBoard->setScale(0.8, 0.8);
    this->addChild(gameBoard, 0);
   
    auto surrenderBtn = ui::Button::create("res/Surrender.png");
    surrenderBtn->setAnchorPoint(Vec2(0.5, 0.5));
    surrenderBtn->addClickEventListener(CC_CALLBACK_1(InGame::Surrender, this));
    surrenderBtn->setPosition(Vec2(visibleSize.width - surrenderBtn->getContentSize().width + origin.x,
        surrenderBtn->getContentSize().height + origin.y));
    this->addChild(surrenderBtn, 15);

    //보드판 생성
    for (int r = 0; r < boardR; r++)
    {
        for (int c = 0; c < boardC; c++)
        {
            (*piece)[r][c] = Piece::create(r, c);
            auto p = (*piece)[r][c];
            p->setAnchorPoint(Vec2(0.5, 0.5));
            p->setPosition(Vec2(90 + r * 16.65 + origin.x, 10 + c* 16.65 + origin.y));
            p->setScale(0.8, 0.8);
            this->addChild(p, 0);
        }
    }

    //클릭 금지
    dontClick = DontClick::create();
    this->addChild(dontClick, 10);

    gameResultScreen = GameResultScreen::create();
    this->addChild(gameResultScreen, 20);

    return true;
}

void InGame::Start()
{
    Message message(MessageType::GAMEBOARD_DATA_REQUEST);
    MESSAGE_MGR.SendMsg(message);

    //게임보드 초기화
    pieceData->clear();

    //초기화
    for (int r = 0; r < boardR; r++)
    {
        for (int c = 0; c < boardC; c++)
        {
            (*piece)[r][c]->Update(PieceType::EMPTY);
        }
    }

    //시작
    for (int r = 0; r < boardR; r++)
    {
        for (int c = 0; c < boardC; c++)
        {
            (*piece)[r][c]->StartSchedule();
        }
    }

    dontClick->StartSchedule();
    
    gameResultScreen->Update(GameResult::EMPTY);
    
    gameResultScreen->StartSchedule();
}

void InGame::Surrender(Ref* ref)
{
    Message message(MessageType::GAME_RESULT_REQUEST);
    message.WriteMessage((int)GameResult::DEFEAT);
    
    MESSAGE_MGR.SendMsg(message);
}

void InGame::UpdateGameBoard(Message& message)
{
    //게임보드 초기화
    pieceData->clear();

    //현재턴 여부
    yourTurn = std::stoi(message.ReadMessage());
    dontClick->Update(yourTurn);

    int cnt = std::stoi(message.ReadMessage());
    for (int i = 0; i < cnt; i++)
    {
        int r = std::stoi(message.ReadMessage());
        int c = std::stoi(message.ReadMessage());
        PieceType pieceType = (PieceType)std::stoi(message.ReadMessage());

        pieceData->push_back(PieceData(r,c, pieceType));
    }

    //초기화
    for (int r = 0; r < boardR; r++)
    {
        for (int c = 0; c < boardC; c++)
        {
            (*piece)[r][c]->Update(PieceType::EMPTY);
        }
    }

    for (int i = 0; i < pieceData->size(); i++)
    {
        int r = (*pieceData)[i].r;
        int c = (*pieceData)[i].c;
        PieceType pieceType = (*pieceData)[i].pieceType;
        (*piece)[r][c]->Update(pieceType);
    }
}

void InGame::UpdateGameResult(Message& messag)
{
    //게임결과
    GameResult yourState = (GameResult)stoi(messag.ReadMessage());
    gameResultScreen->Update(yourState);
}
