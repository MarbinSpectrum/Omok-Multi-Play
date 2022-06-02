#include "GameRoom.h"

GameRoom::GameRoom(std::string roomName, uint roomNum, int64 roomKey, ClientObj* host)
: roomName(roomName)
, roomNum(roomNum)
, roomKey(roomKey)
, host(host)
, gameRun(false)
{
	clientList.insert(host);
	clientReady.insert({{host->socket,host->playerName},false });
}

GameRoom::~GameRoom()
{
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
	clientReady.insert({{guest->socket, guest->playerName}, false });
	if (host == NULL)
	{
		host = guest;
	}

	//���� �濡 �ִ� �ο��鿡�� �������� ���ŉ����� ����
	BroadCastRoomData(guest);

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
	clientReady.erase({ guest->socket, guest->playerName });

	int playerCnt = CountPlayer();
	if (playerCnt == 0)
	{
		//�濡 �ο��� ����.
		//�ش� ���� ������.
		GAMEROOM_MGR.RemoveRoom(roomNum, roomKey);
		return true;
	}

	for (std::set<ClientObj*>::iterator iter = clientList.begin();
		iter != clientList.end(); iter++)
	{
		ClientObj* guestClient = (*iter);
		if (guestClient != NULL)
		{
			//�ƹ� �ο��ϳ��� �������� �����ϰ�
			//�ش� Ŭ���̾�Ʈ�� ��Ͽ��� ����
			host = guestClient;
			clientList.erase(guestClient);
			clientReady.erase({ guestClient->socket, guestClient->playerName });
			break;
		}
	}

	//���ο��鿡�� �������� ���ŉ����� ����
	BroadCastRoomData();

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
void GameRoom::WriteRoomData(Message& message)
{
	string hostName = host->playerName;
	message.WriteMessage(hostName);

	uint playerCnt = CountPlayer() - 1;
	message.WriteMessage(UintToString(playerCnt));

	for (std::set<ClientObj*>::iterator iter = clientList.begin();
		iter != clientList.end(); iter++)
	{
		ClientObj* client = (*iter);
		if (client != NULL && IsHost(client) == false)
		{
			string playerName = client->playerName;
			bool ready = IsPlayerReady(client);

			message.WriteMessage(playerName);
			message.WriteMessage(ready);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
/// : ���ο��鿡�� �������� �ٲ������ �����Ѵ�.
////////////////////////////////////////////////////////////////////////////////
void GameRoom::BroadCastRoomData(ClientObj* ignore)
{
	Message message(MessageType::GAMEROOM_DATA_REPLY);
	message.WriteMessage(1);
	WriteRoomData(message);

	for (std::set<ClientObj*>::iterator iter = clientList.begin();
		iter != clientList.end(); iter++)
	{
		ClientObj* client = (*iter);
		if (client != NULL)
		{
			if (ignore != NULL && client->socket == ignore->socket)
			{
				continue;
			}

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

////////////////////////////////////////////////////////////////////////////////
/// : �÷��̾� �غ񿩺�
////////////////////////////////////////////////////////////////////////////////
bool GameRoom::IsPlayerReady(ClientObj* guest)
{
	return clientReady[{ guest->socket, guest->playerName }];
}
