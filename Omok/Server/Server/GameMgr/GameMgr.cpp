#include "GameMgr.h"
#include "../../../Algorithm/Algorithm.h"
#include <string>
#include "../MsgMgr/MsgMgr.h"

GameMgr::GameMgr()
: gameBoard(new GameBoardMap)
, pieceClient(new PieceClient)
, playerPiece(new PlayerPiece)
, nowTurn(PieceType::BLACK)
, gameRun(false)
{
	
}

GameMgr::~GameMgr()
{
}

void GameMgr::GameStart(ClientObj* player0, ClientObj* player1)
{
	gameRun = true;
	ClearBoard();
	SetPlayerNum(player0, player1);
}

bool GameMgr::IsGameRun()
{
	return gameRun;
}

void GameMgr::SetPlayerNum(ClientObj* player0, ClientObj* player1)
{
	srand(time(NULL));
	int random = rand() % 2;

	if (random == 0)
	{
		SetPlayerPiece(player0, PieceType::BLACK);
		SetPlayerPiece(player1, PieceType::WHITE);
	}
	else
	{
		SetPlayerPiece(player0, PieceType::WHITE);
		SetPlayerPiece(player1, PieceType::BLACK);
	}
}

void GameMgr::SetPlayerPiece(ClientObj* player, PieceType pPieceType)
{
	(*pieceClient)[pPieceType] = player;
	(*playerPiece)[player->clientKey] = pPieceType;
}

bool GameMgr::SetPiece(int r, int c, PieceType pPieceType)
{
	if (gameRun == false)
	{
		//���� �������� �ƴϴ�
		return false;
	}

	if (nowTurn == pPieceType)
	{
		//�ش� �÷��̾��� ���� �ƴϴ�.
		return false;
	}

	(*gameBoard)[r][c] = pPieceType;
	return true;
}

void GameMgr::NextTurnPlayer()
{
	if (nowTurn == PieceType::BLACK)
	{
		nowTurn = PieceType::WHITE;
	}
	else if (nowTurn == PieceType::WHITE)
	{
		nowTurn = PieceType::BLACK;
	}
}

GameResult GameMgr::CheckGameEnd(int r, int c, PieceType pPieceType)
{
	if (gameRun == false)
	{
		//���� �������� �ƴϴ�
		return GameResult::EMPTY;
	}

	int Dic[8][2] = { {1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1} };

	for (int i = 0; i < 8; i++)
	{
		//8������ �˻�
		int ar = r;
		int ac = c;
		bool flag = true;
		for (int j = 0; j < 5; j++)
		{
			//�������� �˻�
			if (GetPiece(r + Dic[i][0] * j, c + Dic[i][1] * j) != pPieceType)
			{
				flag = false;
				break;
			}
		}

		if (flag)
		{
			gameRun = false;
			return GameResult::VICTORY;
		}
	}

	if (GetPieceCount() == 19 * 19)
	{
		return GameResult::DRAW;
	}

	return GameResult::EMPTY;
}

PieceType GameMgr::GetPiece(int r, int c)
{
	if (gameBoard->find(r) == gameBoard->end())
	{
		return PieceType::EMPTY;
	}

	if ((*gameBoard)[r].find(c) == (*gameBoard)[r].end())
	{
		return PieceType::EMPTY;
	}

	return (*gameBoard)[r][c];
}

PieceType GameMgr::GetPiece(ClientObj* player)
{
	int playerKey = player->clientKey;
	if (playerPiece->find(playerKey) != playerPiece->end())
	{
		return (*playerPiece)[player->clientKey];
	}
	return PieceType::EMPTY;
}

uint GameMgr::GetPieceCount()
{
	uint cnt = 0;
	for (GameBoardMap::iterator iter = gameBoard->begin(); iter != gameBoard->end(); iter++)
	{
		//���忡 �ִ� �⹰�� ������ �����´�.
		cnt += (*iter).second.size();
	}
	return cnt;
}

void GameMgr::BroadCastBoardData(ClientObj* ignore)
{
	for (PieceClient::iterator iter = pieceClient->begin();
		iter != pieceClient->end(); iter++)
	{
		ClientObj* client = iter->second;
		if (client != NULL)
		{
			if (ignore != NULL && client->clientKey == ignore->clientKey)
			{
				continue;
			}

			Message msg(MessageType::GAMEBOARD_DATA_REPLY);
			msg.WriteMessage(1);
			WriteNowBoard(msg, client);

			printf("GAMEBOARD_DATA_REPLY %d\n", 1);
			MSG_MGR.SendMsg(msg, client->socket);
		}
	}
}

void GameMgr::BroadCastGameResult(GameResult pGameResult, ClientObj* targetPlayer)
{
	//��������
	gameRun = false;

	for (PieceClient::iterator iter = pieceClient->begin();
		iter != pieceClient->end(); iter++)
	{
		ClientObj* client = iter->second;
		if (client != NULL)
		{
			Message msg(MessageType::GAME_RESULT_REPLY);
			msg.WriteMessage(1);
			if (pGameResult == GameResult::DRAW)
			{
				msg.WriteMessage((int)pGameResult);
			}
			else if (pGameResult == GameResult::VICTORY)
			{
				if (targetPlayer != NULL && client->clientKey == targetPlayer->clientKey)
				{
					msg.WriteMessage((int)GameResult::VICTORY);
				}
				else
				{
					msg.WriteMessage((int)GameResult::DEFEAT);
				}
			}
			else
			{
				if (targetPlayer != NULL && client->clientKey == targetPlayer->clientKey)
				{
					msg.WriteMessage((int)GameResult::DEFEAT);
				}
				else
				{
					msg.WriteMessage((int)GameResult::VICTORY);
				}
			}

			printf("GAME_RESULT_REPLY %d\n", 1);
			MSG_MGR.SendMsg(msg, client->socket);
		}
	}
}

void GameMgr::WriteNowBoard(Message& message, ClientObj* player)
{
	ClientObj* turnPlayer = (*pieceClient)[nowTurn];

	bool yourTurn = (turnPlayer->clientKey == player->clientKey);
	message.WriteMessage(yourTurn);

	//���忡 �ִ� �⹰�� ������ �����´�.
	int cnt = GetPieceCount();
	message.WriteMessage(cnt);

	for (GameBoardMap::iterator iter0 = gameBoard->begin(); iter0 != gameBoard->end(); iter0++)
	{
		for (std::unordered_map<int, PieceType>::iterator iter1 = iter0->second.begin();
			iter1 != iter0->second.end(); iter1++)
		{
			//���� �ʵ忡 �ִ� ��� �⹰�� ��ġ�� �޾ƿ´�.
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



