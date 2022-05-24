#include "GameRoom.h"

GameRoom::GameRoom()
{
}

GameRoom::~GameRoom()
{
}

uint GameRoom::CountPlayer()
{
	uint num = 0;
	if (player0 != NULL)
	{
		num++;
	}
	if (player1 != NULL)
	{
		num++;
	}
	return num;
}