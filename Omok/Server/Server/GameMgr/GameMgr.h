#pragma once

#include <unordered_map>
#include <map>
#include "../../../Message/Message.h"
#include "../ClientMgr/ClientMgr.h"

typedef __int64 int64;
typedef unsigned int uint;

enum class PieceType
{
	EMPTY	= 0,
	WHITE	= 1,
	BLACK	= 2
};

enum class GameResult
{
	EMPTY		= 0,
	VICTORY		= 1,
	DEFEAT		= 2,
	DRAW		= 100,
};



class GameMgr
{
	typedef  std::unordered_map<int, std::unordered_map<int, PieceType>> GameBoardMap;
	typedef  std::unordered_map<PieceType, ClientObj*> PieceClient;
	typedef  std::unordered_map<int64, PieceType> PlayerPiece;
public:
	GameMgr();
	~GameMgr();

public:
	void			GameStart(ClientObj* player0, ClientObj* player1);
	bool			IsGameRun();
	void			SetPlayerNum(ClientObj* player0, ClientObj* player1);
	void			SetPlayerPiece(ClientObj* player, PieceType pPieceType);
	bool			SetPiece(int r, int c, PieceType pPieceType);
	void			NextTurnPlayer();
	GameResult		CheckGameEnd(int r, int c, PieceType pPieceType);
	PieceType		GetPiece(int r, int c);
	PieceType		GetPiece(ClientObj* player);
	uint			GetPieceCount();
	void			BroadCastBoardData(ClientObj* ignore = NULL);
	void			BroadCastGameResult(GameResult pGameResult, ClientObj* targetPlayer = NULL);
	void			WriteNowBoard(Message& message, ClientObj* player);
	void			ClearBoard();

private:
	GameBoardMap*	gameBoard;
	PieceClient*	pieceClient;
	PlayerPiece*	playerPiece;

	PieceType		nowTurn;
	bool			gameRun;
};
