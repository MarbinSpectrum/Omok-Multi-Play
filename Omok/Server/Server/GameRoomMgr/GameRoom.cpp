#include "GameRoom.h"

GameRoom::GameRoom(std::string roomName, uint roomNum, int64 roomKey, ClientObj* host)
: roomName(roomName)
, roomNum(roomNum)
, roomKey(roomKey)
, host(host)
, gameMgr(new GameMgr())
, gameRun(false)
{
	clientList.insert(host);
	clientReady.insert({ host->clientKey,false });
}

GameRoom::~GameRoom()
{
	if (gameMgr != NULL)
	{
		delete gameMgr;
		gameMgr = NULL;
	}
}

////////////////////////////////////////////////////////////////////////////////
/// : 현재 방의 인원 수를 가져온다.
////////////////////////////////////////////////////////////////////////////////
uint GameRoom::CountPlayer()
{
	return clientList.size();
}

////////////////////////////////////////////////////////////////////////////////
/// : 해당 인원이 방에 존재하는지 검사한다.
////////////////////////////////////////////////////////////////////////////////
bool GameRoom::ExistRoom(ClientObj* guest)
{
	for (std::set<ClientObj*>::iterator iter = clientList.begin(); 
		iter != clientList.end(); iter++)
	{
		if (guest->socket == (*iter)->socket)
		{
			return true;
		}
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////
/// : 방에 입장을 요청하고 등록한다.
////////////////////////////////////////////////////////////////////////////////
bool GameRoom::EnterGameRoom(ClientObj* guest)
{
	if (CountPlayer() >= GameRoomMgr::MaxPersonNum)
	{
		//방에 공간이 없다.
		return false;
	}

	if (guest == NULL)
	{
		return false;
	}

	if (ExistRoom(guest))
	{
		//방에 이미 존재한다.
		return false;
	}

	if (gameRun)
	{
		//게임이 진행중이다.
		return false;
	}


	//클라이언트 정보설정
	guest->playerPos = { roomNum,roomKey };
	clientList.insert(guest);
	clientReady.insert({guest->clientKey, false });
	if (host == NULL)
	{
		host = guest;
	}

	if (gameRun == false)
	{
		//현재 게임중이 아니라면
		//현재 방에 있는 인원들에게 방정보가 갱신됬음을 전달
		BroadCastRoomData(guest);
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////
/// : 방을 나가는 것을 요청
////////////////////////////////////////////////////////////////////////////////
bool GameRoom::ExitGameRoom(ClientObj* guest)
{
	if (ExistRoom(guest) == false)
	{
		return false;
	}

	guest->ExitRoom();
	if (IsHost(guest))
	{
		//호스트를 제거
		host = NULL;
	}

	//해당 인원을 리스트에서 삭제
	clientList.erase(guest);
	clientReady.erase(guest->clientKey);

	int playerCnt = CountPlayer();
	if (playerCnt == 0)
	{
		//방에 인원이 없다.
		//해당 방을 없애자.
		GAMEROOM_MGR.RemoveRoom(roomNum, roomKey);
		return true;
	}

	if (host == NULL)
	{
		for (std::set<ClientObj*>::iterator iter = clientList.begin();
			iter != clientList.end(); iter++)
		{
			ClientObj* guestClient = (*iter);
			if (guestClient != NULL)
			{
				//아무 인원하나를 방장으로 지정하고
				host = guestClient;
				break;
			}
		}
	}

	if (gameRun == false)
	{
		//현재 게임중이 아니라면
		//방인원들에게 방정보가 갱신됬음을 전달
		BroadCastRoomData();
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////
/// : 방 외부 정보(방 이름, 방번호, 방키값, 방인원수)
////////////////////////////////////////////////////////////////////////////////
void GameRoom::WriteLobbyRoomData(Message& message)
{
	message.WriteMessage(roomName);
	message.WriteMessage(to_string(roomNum));
	message.WriteMessage(to_string(roomKey));
	message.WriteMessage(to_string(CountPlayer()));
}

////////////////////////////////////////////////////////////////////////////////
/// : 방 내부 정보(방장, 플레이어 준비상태)
////////////////////////////////////////////////////////////////////////////////
void GameRoom::WriteRoomData(Message& message, SOCKET socket)
{
	string hostName = host->playerName;
	message.WriteMessage(hostName);

	ClientObj* hostClient = CLIENT_MGR.GetClient(socket);
	bool hostPlayer = IsHost(hostClient);
	message.WriteMessage(hostPlayer);

	uint playerCnt = CountPlayer() - 1;
	message.WriteMessage(UintToString(playerCnt));

	for (std::set<ClientObj*>::iterator iter = clientList.begin();
		iter != clientList.end(); iter++)
	{
		ClientObj* client = (*iter);
		if (client != NULL && IsHost(client) == false)
		{
			string playerName = client->playerName;
			bool ready = GetIsPlayerReady(client);

			message.WriteMessage(playerName);
			message.WriteMessage(ready);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
/// : 플레이어 준비여부
////////////////////////////////////////////////////////////////////////////////
bool GameRoom::GetIsPlayerReady(ClientObj* guest)
{
	return clientReady[guest->clientKey];
}

////////////////////////////////////////////////////////////////////////////////
/// : 플레이어 준비여부를 설정
////////////////////////////////////////////////////////////////////////////////
void GameRoom::SetIsPlayerReady(ClientObj* guest, bool state)
{
	bool nowState = GetIsPlayerReady(guest);
	if (nowState == state)
	{
		return;
	}
	clientReady[guest->clientKey] = state;
	BroadCastRoomData();
}

////////////////////////////////////////////////////////////////////////////////
/// : 게임시작
////////////////////////////////////////////////////////////////////////////////
void GameRoom::GameStart()
{
	ClientObj* client1 = NULL;
	ClientObj* client2 = NULL;
	for (std::set<ClientObj*>::iterator iter = clientList.begin();
		iter != clientList.end(); iter++)
	{
		ClientObj* client = (*iter);
		if (client != NULL)
		{
			if (client1 == NULL)
			{
				client1 = client;
			}
			else if (client2 == NULL)
			{
				client2 = client;
			}
		}
	}
	gameMgr->GameStart(client1, client2);

	Message message(MessageType::GAMEROOM_GAME_START_REPLY);
	message.WriteMessage(1);

	MSG_MGR.SendMsg(message, client1->socket);
	MSG_MGR.SendMsg(message, client2->socket);
}

////////////////////////////////////////////////////////////////////////////////
/// : 플레이어 준비여부
////////////////////////////////////////////////////////////////////////////////
GameMgr* GameRoom::GetGameMgr()
{
	return gameMgr;
}

////////////////////////////////////////////////////////////////////////////////
/// : 방인원들에게 같은 메세지를 모두 보내준다.
////////////////////////////////////////////////////////////////////////////////
void GameRoom::BroadCastBoardData(ClientObj* ignore)
{
	for (std::set<ClientObj*>::iterator iter = clientList.begin();
		iter != clientList.end(); iter++)
	{
		ClientObj* client = (*iter);
		if (client != NULL)
		{
			if (ignore != NULL && client->clientKey == ignore->clientKey)
			{
				continue;
			}

			Message msg(MessageType::GAMEBOARD_DATA_REPLY);
			msg.WriteMessage(1);
			gameMgr->WriteNowBoard(msg, client);

			printf("GAMEBOARD_DATA_REPLY %d\n", 1);
			MSG_MGR.SendMsg(msg, client->socket);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
/// : 방인원들에게 방정보가 바뀌었음을 전달한다.
////////////////////////////////////////////////////////////////////////////////
void GameRoom::BroadCastRoomData(ClientObj* ignore)
{
	for (std::set<ClientObj*>::iterator iter = clientList.begin();
		iter != clientList.end(); iter++)
	{
		ClientObj* client = (*iter);
		if (client != NULL)
		{
			if (ignore != NULL && client->clientKey == ignore->clientKey)
			{
				continue;
			}

			Message message(MessageType::GAMEROOM_DATA_REPLY);
			message.WriteMessage(1);
			WriteRoomData(message, client->socket);

			MSG_MGR.SendMsg(message, client->socket);		
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
/// : 호스트여부
////////////////////////////////////////////////////////////////////////////////
bool GameRoom::IsHost(ClientObj* guest)
{
	if (host->socket == guest->socket)
	{
		return true;
	}
	return false;
}