#include "MassageMgr.h"

MassageMgr::MassageMgr()
{
	//������
}

MassageMgr::~MassageMgr()
{
	//�Ҹ���
}

MassageMgr* MassageMgr::instance = NULL;
MassageMgr& MassageMgr::Instance()
{
	if (instance == NULL)
	{
		//�̱��� ��ü�� �߰�
		instance = new MassageMgr();

		instance->Init();
	}
	return *instance;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : �Ŵ��� �ʱ�ȭ
////////////////////////////////////////////////////////////////////////////////////////////////
void MassageMgr::Init()
{

}

void MassageMgr::SendMessage(Message message)
{

}
