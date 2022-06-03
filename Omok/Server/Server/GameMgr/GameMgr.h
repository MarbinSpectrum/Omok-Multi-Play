#pragma once

#include <unordered_map>
#include <map>
#include "../../../Message/Message.h"
#include "../ClientMgr/ClientMgr.h"

typedef __int64 int64;
typedef unsigned int uint;

enum class PieceType
{
	EMPTY,
	WHITE,
	BLACK
};

class GameMgr
{
	typedef  uint player_num;
	typedef  std::unordered_map<int, std::unordered_map<int, PieceType>> GameBoardMap;
	typedef  std::unordered_map<SOCKET, player_num> PlayerNum;
	typedef  std::unordered_map<player_num, PieceType> PlayerPiece;
public:
	GameMgr();
	~GameMgr();

public:
	void			GameStart(ClientObj* player0, ClientObj* player1);
	void			SetPlayerNum(ClientObj* player0, ClientObj* player1);
	void			SetPlayerNum(ClientObj* player, player_num num);
	void			SetPlayerPiece(player_num playerNum, PieceType pPieceType);
	bool			SetPiece(int r, int c, PieceType pPieceType);
	void			SendGameResult();
	void			WriteNowBoard(Message& message, ClientObj* player);
	void			ClearBoard();

public:
	GameBoardMap*	gameBoard;
	PlayerNum*		playerNum;
	PlayerPiece*	playerPiece;

	PieceType		nowTurn;
};
