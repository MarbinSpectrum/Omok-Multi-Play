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
/// : ���� ���� �ο� ���� �����´�.
////////////////////////////////////////////////////////////////////////////////
uint GameRoom::CountPlayer()
{
	return clientList.size();
}

////////////////////////////////////////////////////////////////////////////////
/// : �ش� �ο��� �濡 �����ϴ��� �˻��Ѵ�.
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
/// : �濡 ������ ��û�ϰ� ����Ѵ�.
////////////////////////////////////////////////////////////////////////////////
bool GameRoom::EnterGameRoom(ClientObj* guest)
{
	if (CountPlayer() >= GameRoomMgr::MaxPersonNum)
	{
		//�濡 ������ ����.
		return false;
	}

	if (guest == NULL)
	{
		return false;
	}

	if (ExistRoom(guest))
	{
		//�濡 �̹� �����Ѵ�.
		return false;
	}

	if (gameRun)
	{
		//������ �������̴�.
		return false;
	}


	//Ŭ���̾�Ʈ ��������
	guest->playerPos = { roomNum,roomKey };
	clientList.insert(guest);
	clientReady.insert({guest->clientKey, false });
	if (host == NULL)
	{
		host = guest;
	}

	if (gameRun == false)
	{
		//���� �������� �ƴ϶��
		//���� �濡 �ִ� �ο��鿡�� �������� ���ŉ����� ����
		BroadCastRoomData(guest);
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////
/// : ���� ������ ���� ��û
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
		//ȣ��Ʈ�� ����
		host = NULL;
	}

	//�ش� �ο��� ����Ʈ���� ����
	clientList.erase(guest);
	clientReady.erase(guest->clientKey);

	int playerCnt = CountPlayer();
	if (playerCnt == 0)
	{
		//�濡 �ο��� ����.
		//�ش� ���� ������.
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
				//�ƹ� �ο��ϳ��� �������� �����ϰ�
				host = guestClient;
				break;
			}
		}
	}

	if (gameRun == false)
	{
		//���� �������� �ƴ϶��
		//���ο��鿡�� �������� ���ŉ����� ����
		BroadCastRoomData();
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////
/// : �� �ܺ� ����(�� �̸�, ���ȣ, ��Ű��, ���ο���)
////////////////////////////////////////////////////////////////////////////////
void GameRoom::WriteLobbyRoomData(Message& message)
{
	message.WriteMessage(roomName);
	message.WriteMessage(to_string(roomNum));
	message.WriteMessage(to_string(roomKey));
	message.WriteMessage(to_string(CountPlayer()));
}

////////////////////////////////////////////////////////////////////////////////
/// : �� ���� ����(����, �÷��̾� �غ����)
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
/// : �÷��̾� �غ񿩺�
////////////////////////////////////////////////////////////////////////////////
bool GameRoom::GetIsPlayerReady(ClientObj* guest)
{
	return clientReady[guest->clientKey];
}

////////////////////////////////////////////////////////////////////////////////
/// : �÷��̾� �غ񿩺θ� ����
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
/// : ���ӽ���
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
/// : �÷��̾� �غ񿩺�
////////////////////////////////////////////////////////////////////////////////
GameMgr* GameRoom::GetGameMgr()
{
	return gameMgr;
}

////////////////////////////////////////////////////////////////////////////////
/// : ���ο��鿡�� ���� �޼����� ��� �����ش�.
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
/// : ���ο��鿡�� �������� �ٲ������ �����Ѵ�.
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
/// : ȣ��Ʈ����
////////////////////////////////////////////////////////////////////////////////
bool GameRoom::IsHost(ClientObj* guest)
{
	if (host->socket == guest->socket)
	{
		return true;
	}
	return false;
}