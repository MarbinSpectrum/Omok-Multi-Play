#include "MassageMgr.h"

MassageMgr::MassageMgr()
{
	//»ı¼ºÀÚ
}

MassageMgr::~MassageMgr()
{
	//¼Ò¸êÀÚ
}

MassageMgr* MassageMgr::instance = NULL;
MassageMgr& MassageMgr::Instance()
{
	if (instance == NULL)
	{
		//½Ì±ÛÅæ °´Ã¼¸¦ Ãß°¡
		instance = new MassageMgr();

		instance->Init();
	}
	return *instance;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : ¸Å´ÏÀú ÃÊ±âÈ­
////////////////////////////////////////////////////////////////////////////////////////////////
void MassageMgr::Init()
{

}

void MassageMgr::SendMessage(Message message)
{

}
