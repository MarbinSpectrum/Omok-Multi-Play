#include "PlayerMgr.h"

PlayerMgr::PlayerMgr()
{
	//������
}

PlayerMgr::~PlayerMgr()
{
	//�Ҹ���
}

PlayerMgr* PlayerMgr::instance = NULL;
PlayerMgr& PlayerMgr::Instance()
{
	if (instance == NULL)
	{
		//�̱��� ��ü�� �߰�
		instance = new PlayerMgr();
	}
	return *instance;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : �Ŵ��� �ʱ�ȭ
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
