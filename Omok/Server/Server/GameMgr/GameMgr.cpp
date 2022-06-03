#include "GameMgr.h"
#include "../../../Algorithm/Algorithm.h"
#include <string>

GameMgr::GameMgr()
: gameBoard(new GameBoardMap)
//, playerNum(new PlayerNum)
//, playerPiece(new PlayerPiece)
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
void GameMgr::SetPlayerNum(ClientObj* player, uint num)
{
	SOCKET socket = player->socket;
	std::string playerName = player->playerName;
	//(*playerNum)[{playerName, socket}] = num;
}

void GameMgr::SetPlayerPiece(uint playerNum, PieceType pPieceType)
{
	//(*playerPiece)[playerNum] = pPieceType;
}

bool GameMgr::SetPiece(uint r, uint c, PieceType pPieceType)
{
	uint pos = MakeSetPos(r, c);
	if (nowTurn == pPieceType)
	{
		//현재 턴에 해당하는 바둑알
		if (gameBoard->find(pos) == gameBoard->end())
		{
			//바둑알을 배치
			(*gameBoard)[pos] = pPieceType;
			return true;
		}
	}
	return false;
}

void GameMgr::SendGameResult()
{
}

void GameMgr::WriteNowBoard(Message& message)
{
	int cnt = gameBoard->size();
	message.WriteMessage(cnt);

	/*for (GameBoardMap::iterator iter = gameBoard->begin(); iter != gameBoard->end(); iter++)
	{
		uint r = iter->first.first;
		uint c = iter->first.second;
		PieceType pieceType = iter->second;
		message.WriteMessage(UintToString(r));
		message.WriteMessage(UintToString(c));
		message.WriteMessage(std::to_string((int)pieceType));
	}*/
}

void GameMgr::ClearBoard()
{
	gameBoard->clear();
}

inline uint GameMgr::MakeSetPos(uint r, uint c)
{
	return r * 1000 + c;
}

