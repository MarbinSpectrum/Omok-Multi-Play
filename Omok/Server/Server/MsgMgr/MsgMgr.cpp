#include "MsgMgr.h"
#include "../ClientMgr/ClientMgr.h"

MsgMgr* MsgMgr::instance = NULL;
MsgMgr& MsgMgr::Instance()
{
	if (instance == NULL)
	{
		//싱글톤 객체를 추가
		instance = new MsgMgr();
	}
	return *instance;
}

////////////////////////////////////////////////////////////////////////////////
/// : 메세지를 보내는 부분
////////////////////////////////////////////////////////////////////////////////
void MsgMgr::SendMsg(Message message, SOCKET socket)
{
	std::string msg = message.ConvertString();
	send(socket, msg.c_str(), strlen(msg.c_str()), 0);
}

////////////////////////////////////////////////////////////////////////////////
/// : 메세지를 받는 부분
////////////////////////////////////////////////////////////////////////////////
void MsgMgr::OnReceiveMsg(Message message, SOCKET socket)
{
	MessageType messageType = message.GetMessageType();

	switch (messageType)
	{
		case MessageType::CONNECT_CLIENT:
		{
			//클라이언트와 서버가 연결됨
			//소캣번호
			string sk = message.ReadMessage();
			printf("CONNECT_CLIENT SOCKET : %s\n", sk.c_str());
		}
		break;
		case MessageType::DISCONNECT_CLIENT:
		{
			//클라이언트와 서버가 연결이 해제됨
			//소캣번호
			string sk = message.ReadMessage();
			printf("DISCONNECT_CLIENT SOCKET : %s\n", sk.c_str());

			CLIENT_MGR.RemoveClient(socket);
		}
		break;
		case MessageType::LOBBY_ENTER_REQUEST:
		{
			//클라이언트의 로비 입장 요청
			//아이디
			std::string playerName = message.ReadMessage();
			printf("LOBBY_ENTER_REQUEST 아이디 : %s\n", playerName.c_str());

			bool success = false;
			Message msg(MessageType::LOBBY_ENTER_REPLY);

			if (ClientMgr::PlayerNameCheck(playerName) == false)
			{
				//아이디가 이상함
				msg.WriteMessage(false);
				success = false;
			}
			else if (CLIENT_MGR.RegistClient(socket, playerName))
			{
				//등록이 성공적으로 완료됨
				msg.WriteMessage(true);
				msg.WriteMessage(playerName);
				success = true;
			}
			else
			{
				//등록이 안됨
				msg.WriteMessage(false);
				success = false;
			}

			printf("LOBBY_ENTER_REPLY %d\n", success);
			SendMsg(msg, socket);
		}
		break;
		case MessageType::LOBBY_ROOM_DATA_REQUEST:
		{
			//로비의 방 데이터를 요청
			//아이디
			printf("LOBBY_ROOM_DATA_REQUEST\n");

			Message msg(MessageType::LOBBY_ROOM_DATA_REPLY);
			GAMEROOM_MGR.WriteRoomDatas(msg);

			printf("LOBBY_ROOM_DATA_REPLY\n");
			SendMsg(msg, socket);
		}
		break;
		case MessageType::MAKE_ROOM_REQUEST:
		{
			//방을 만드는 것을 요청
			printf("MAKE_ROOM_REQUEST\n");
			bool success = GAMEROOM_MGR.CreateRoom(socket);

			Message msg(MessageType::MAKE_ROOM_REPLY);
			msg.WriteMessage(success);

			printf("MAKE_ROOM_REPLY %d\n", success);
			SendMsg(msg, socket);
		}
		break;
		case MessageType::ENTER_ROOM_REQUEST:
		{
			//방에 입장하는 것을 요청
			//방 번호, 키
			printf("ENTER_ROOM_REQUEST\n");
			uint roomNum = StringToUint(message.ReadMessage());
			int64 roomKey = StringToInt64(message.ReadMessage());

			bool success = GAMEROOM_MGR.EnterRoom(socket, roomNum, roomKey);
			Message msg(MessageType::ENTER_ROOM_REPLY);
			msg.WriteMessage(success);

			printf("ENTER_ROOM_REPLY %d\n", success);
			SendMsg(msg, socket);
		}
		break;
		case MessageType::EXIT_ROOM_REQUEST:
		{
			//방에 퇴장하는 것을 요청
			printf("EXIT_ROOM_REQUEST\n");

			bool success = GAMEROOM_MGR.ExitRoom(socket);

			Message msg(MessageType::EXIT_ROOM_REPLY);
			msg.WriteMessage(success);

			printf("EXIT_ROOM_REPLY %d\n", success);
			SendMsg(msg, socket);
		}
		break;
		case MessageType::GAMEROOM_DATA_REQUEST:
		{
			//방 세부 정보를 요청
			printf("GAMEROOM_DATA_REQUEST\n");

			Message msg(MessageType::GAMEROOM_DATA_REPLY);
			bool success = false;

			ClientObj* clientObj = CLIENT_MGR.GetClient(socket);
			if (clientObj == NULL)
			{
				//클라이언트가 존재하지 않는다.
				success = false;
				msg.WriteMessage(success);
			}
			else
			{
				uint roomNum = clientObj->playerPos.roomNum;
				int64 roomKey = clientObj->playerPos.roomKey;

				GameRoom* gameRoom = GAMEROOM_MGR.GetGameRoom(roomNum, roomKey);
				if (gameRoom == NULL)
				{
					//방이 존재하지 않는다.
					success = false;
					msg.WriteMessage(success);
				}
				else
				{
					success = true;
					msg.WriteMessage(success);
					gameRoom->WriteRoomData(msg, socket);
				}
			}

			printf("GAMEROOM_DATA_REPLY %d\n", success);
			SendMsg(msg, socket);
		}
		break;
		case MessageType::GAMEROOM_READY_UPDATE:
		{
			//게임 준비상태 갱신 요청
			//준비상태(bool)
			printf("GAMEROOM_READY_UPDATE\n");
			ClientObj* clientObj = CLIENT_MGR.GetClient(socket);
			bool state = stoi(message.ReadMessage());

			uint roomNum = clientObj->playerPos.roomNum;
			int64 roomKey = clientObj->playerPos.roomKey;

			GameRoom* gameRoom = GAMEROOM_MGR.GetGameRoom(roomNum, roomKey);
			if (gameRoom != NULL)
			{
				gameRoom->SetIsPlayerReady(clientObj, state);
			}
		}
		break;
		case MessageType::GAMEROOM_GAME_START_REQUEST:
		{
			//게임 시작 요청
			printf("GAMEROOM_GAME_START_REQUEST\n");
			ClientObj* clientObj = CLIENT_MGR.GetClient(socket);

			uint roomNum = clientObj->playerPos.roomNum;
			int64 roomKey = clientObj->playerPos.roomKey;

			GameRoom* gameRoom = GAMEROOM_MGR.GetGameRoom(roomNum, roomKey);
			if (gameRoom != NULL)
			{
				gameRoom->GameStart();
			}
			else
			{
				Message message(MessageType::GAMEROOM_GAME_START_REPLY);
				message.WriteMessage(0);

				printf("GAMEROOM_GAME_START_REPLY\n");
				MSG_MGR.SendMsg(message, socket);
			}
		}
		break;
		case MessageType::GAMEBOARD_DATA_REQUEST:
		{
			//게임판 상태를 요청
			//플레이어의 턴
			//피스 갯수
			//피스 R
			//피스 C
			//피스 종류
			printf("GAMEBOARD_DATA_REQUEST\n");
			ClientObj* clientObj = CLIENT_MGR.GetClient(socket);

			Message msg(MessageType::GAMEBOARD_DATA_REPLY);
			bool success = false;

			if (clientObj == NULL)
			{
				success = false;
				msg.WriteMessage(success);
			}
			else
			{
				uint roomNum = clientObj->playerPos.roomNum;
				int64 roomKey = clientObj->playerPos.roomKey;

				GameRoom* gameRoom = GAMEROOM_MGR.GetGameRoom(roomNum, roomKey);
				if (gameRoom != NULL)
				{
					success = true;
					msg.WriteMessage(success);

					GameMgr* gameMgr = gameRoom->GetGameMgr();
					gameMgr->WriteNowBoard(msg, clientObj);
				}
				else
				{
					success = false;
					msg.WriteMessage(success);
				}
			}

			printf("GAMEBOARD_DATA_REPLY %d\n", success);
			SendMsg(msg, socket);
		}
		break;
		case MessageType::GAMEBOARD_SET_PIECE:
		{
			//게임판에 기물 배치 요청
			//r
			//c
			int pieceR = stoi(message.ReadMessage());
			int pieceC = stoi(message.ReadMessage());

			printf("GAMEBOARD_SET_PIECE\n");

			ClientObj* clientObj = CLIENT_MGR.GetClient(socket);

			if (clientObj != NULL)
			{
				uint roomNum = clientObj->playerPos.roomNum;
				int64 roomKey = clientObj->playerPos.roomKey;

				GameRoom* gameRoom = GAMEROOM_MGR.GetGameRoom(roomNum, roomKey);
				if (gameRoom != NULL)
				{
					GameMgr* gameMgr = gameRoom->GetGameMgr();
					PieceType playerPiece = gameMgr->GetPiece(clientObj);
					if (gameMgr->SetPiece(pieceR, pieceC, playerPiece))
					{
						printf("%d %d %d\n", pieceR, pieceC, playerPiece);
						GameResult gameResult = gameMgr->CheckGameEnd(pieceR, pieceC, playerPiece);
						if (gameResult != GameResult::EMPTY)
						{
							gameMgr->BroadCastGameResult(gameResult, clientObj);
						}
						else
						{
							gameMgr->NextTurnPlayer();
						}
						gameMgr->BroadCastBoardData();
					}
				}
			}
		}
		break;
		case MessageType::GAME_RESULT_REQUEST:
		{
			printf("GAME_RESULT_REQUEST\n");

			GameResult gameResult = (GameResult)stoi(message.ReadMessage());
			if (gameResult == GameResult::DEFEAT)
			{
				ClientObj* clientObj = CLIENT_MGR.GetClient(socket);

				if (clientObj != NULL)
				{
					uint roomNum = clientObj->playerPos.roomNum;
					int64 roomKey = clientObj->playerPos.roomKey;

					GameRoom* gameRoom = GAMEROOM_MGR.GetGameRoom(roomNum, roomKey);
					if (gameRoom != NULL)
					{
						GameMgr* gameMgr = gameRoom->GetGameMgr();

						gameMgr->BroadCastGameResult(gameResult, clientObj);
					}
				}
			}
			break;
		}
	}
}
