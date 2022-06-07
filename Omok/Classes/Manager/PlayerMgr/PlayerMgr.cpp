#include "PlayerMgr.h"

PlayerMgr::PlayerMgr()
{
	//생성자
}

PlayerMgr::~PlayerMgr()
{
	//소멸자
	Message message(MessageType::DISCONNECT_CLIENT);
	message.WriteMessage(to_string(MESSAGE_MGR.GetSock()));
	MESSAGE_MGR.SendMsg(message);
}

PlayerMgr* PlayerMgr::instance = NULL;
PlayerMgr& PlayerMgr::Instance()
{
	if (instance == NULL)
	{
		//싱글톤 객체를 추가
		instance = new PlayerMgr();
	}
	return *instance;
}

void PlayerMgr::Destroy()
{
	delete instance;
	instance = NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : 매니저 초기화
////////////////////////////////////////////////////////////////////////////////////////////////
void PlayerMgr::Init()
{

}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : 이름값 등록
////////////////////////////////////////////////////////////////////////////////////////////////
void PlayerMgr::RegistNickName(std::string nickName)
{
	playerNickName = nickName;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : 이름값 가져오기
////////////////////////////////////////////////////////////////////////////////////////////////
std::string* PlayerMgr::GetNickName()
{
	return &playerNickName;
}
