#include "GameMgr.h"
#include "../../../Algorithm/Algorithm.h"
#include <string>

GameMgr::GameMgr()
: gameBoard(new GameBoardMap)
, playerNum(new PlayerNum)
, playerPiece(new PlayerPiece)
, nowTurn(PieceType::BLACK)
{
	
}

GameMgr::~GameMgr()
{
}

void GameMgr::GameStart(ClientObj* player0, ClientObj* player1)
{
	//nowTurn = PieceType::BLACK;
	SetPlayerNum(player0, player1);
	SetPlayerPiece(1, PieceType::BLACK);
	SetPlayerPiece(2, PieceType::WHITE);
}

void GameMgr::SetPlayerNum(ClientObj* player0, ClientObj* player1)
{
	srand(time(NULL));
	int random = rand() % 2;

	if (random == 0)
	{
		SetPlayerNum(player0, 1);
		SetPlayerNum(player1, 2);
	}
	else
	{
		SetPlayerNum(player0, 2);
		SetPlayerNum(player1, 1);
	}
}
void GameMgr::SetPlayerNum(ClientObj* player, player_num num)
{
	SOCKET socket = player->socket;
	std::string playerName = player->playerName;
	(*playerNum)[socket] = num;
}

void GameMgr::SetPlayerPiece(player_num playerNum, PieceType pPieceType)
{
	(*playerPiece)[playerNum] = pPieceType;
}

bool GameMgr::SetPiece(int r, int c, PieceType pPieceType)
{
	if (nowTurn == pPieceType)
	{
		//해당 플레이어의 턴이 아니다.
		return false;
	}

	if (gameBoard->find(r) == gameBoard->end())
	{
		//바둑알을 배치
		(*gameBoard)[r][c] = pPieceType;
		return true;
	}
	else
	{
		if (gameBoard->find(r)->second.find(c) == gameBoard->find(r)->second.end())
		{
			//바둑알을 배치
			(*gameBoard)[r][c] = pPieceType;
			return true;
		}
	}
	return false;
}

void GameMgr::SendGameResult()
{
}

void GameMgr::WriteNowBoard(Message& message, ClientObj* player)
{
	player_num num = (*playerNum)[player->socket];
	PieceType piece = (*playerPiece)[num];

	message.WriteMessage(piece == nowTurn);

	int cnt = gameBoard->size();
	message.WriteMessage(cnt);

	for (GameBoardMap::iterator iter0 = gameBoard->begin(); iter0 != gameBoard->end(); iter0++)
	{
		for (std::unordered_map<int, PieceType>::iterator iter1 = iter0->second.begin();
			iter1 != iter0->second.end(); iter1++)
		{
			//현재 필드에 있는 모든 기물의 위치를 받아온다.
			int r = iter0->first;
			int c = iter1->first;
			PieceType pieceType = iter1->second;
			message.WriteMessage(UintToString(r));
			message.WriteMessage(UintToString(c));
			message.WriteMessage(std::to_string((int)pieceType));
		}
	}
}

void GameMgr::ClearBoard()
{
	gameBoard->clear();
}

