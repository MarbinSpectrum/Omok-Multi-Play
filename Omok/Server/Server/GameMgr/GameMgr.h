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
	typedef  std::unordered_map<uint, PieceType> GameBoardMap;
	typedef  std::unordered_map<SOCKET, uint> PlayerNum;
	typedef  std::unordered_map<uint, PieceType> PlayerPiece;
public:
	GameMgr();
	~GameMgr();

public:
	void			GameStart(ClientObj* player0, ClientObj* player1);
	void			SetPlayerNum(ClientObj* player0, ClientObj* player1);
	void			SetPlayerNum(ClientObj* player, uint num);
	void			SetPlayerPiece(uint playerNum, PieceType pPieceType);
	bool			SetPiece(uint r, uint c, PieceType pPieceType);
	void			SendGameResult();
	void			WriteNowBoard(Message& message);
	void			ClearBoard();

private:
	inline uint		MakeSetPos(uint r, uint c);

public:
	GameBoardMap*	gameBoard;
	PlayerNum*		playerNum;
	//PlayerPiece*	playerPiece;

	PieceType		nowTurn;
};
