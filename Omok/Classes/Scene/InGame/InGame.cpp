#include "InGame.h"

USING_NS_CC;

Scene* InGame::createScene()
{
    return InGame::create();
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
   
    //보드판 생성
    for (int r = 0; r < boardR; r++)
    {
        for (int c = 0; c < boardC; c++)
        {
            piece[r][c] = Piece::create();
            auto p = piece[r][c];
            p->setAnchorPoint(Vec2(0.5, 0.5));
            p->setPosition(Vec2(90 + r * 16.65 + origin.x, 10 + c* 16.65 + origin.y));
            p->setScale(0.8, 0.8);
            this->addChild(p, 0);
        }
    }

    return true;
}

void InGame::Start()
{
    Message message(MessageType::GAMEBOARD_DATA_REQUEST);
    MASSAGE_MGR.SendMsg(message);

    //게임보드 초기화
    pieceData.clear();

    this->schedule(CC_SCHEDULE_SELECTOR(InGame::UpdateInGame), 0.03f);
}

void InGame::UpdateGameBoard(PieceDatas& pPieceData)
{
    //게임보드 초기화
    pieceData.clear();
    for (int i = 0; i < pPieceData.size(); i++)
    {
        int r = pPieceData[i].first.first;
        int c = pPieceData[i].first.second;
        PieceType pieceType = pPieceData[i].second;
        pieceData.push_back({ { r,c }, pieceType });
    }
}

void InGame::UpdateInGame(float f)
{
    //초기화
    for (int r = 0; r < boardR; r++)
    {
        for (int c = 0; c < boardC; c++)
        {
            piece[r][c]->Update(PieceType::EMPTY);
        }
    }

    for (int i = 0; i < pieceData.size(); i++)
    {
        int r = pieceData[i].first.first;
        int c = pieceData[i].first.second;
        PieceType pieceType = pieceData[i].second;
        piece[r][c]->Update(pieceType);
    }
}
