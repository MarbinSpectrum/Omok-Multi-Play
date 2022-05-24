#include "PlayerMgr.h"

PlayerMgr::PlayerMgr()
{
	//생성자
}

PlayerMgr::~PlayerMgr()
{
	//소멸자
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

////////////////////////////////////////////////////////////////////////////////////////////////
/// : 매니저 초기화
////////////////////////////////////////////////////////////////////////////////////////////////
void PlayerMgr::Init()
{

}

void PlayerMgr::RegistNickName(std::string nickName)
{
	playerNickName = nickName;
}

std::string* PlayerMgr::GetNickName()
{
	return &playerNickName;
}
