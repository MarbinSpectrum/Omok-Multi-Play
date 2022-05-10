#include "SceneMgr.h"

SceneMgr::SceneMgr() 
: sceneMap(new SceneMap)
{
	//»ý¼ºÀÚ
}

SceneMgr::~SceneMgr()
{
	//¼Ò¸êÀÚ

	if (sceneMap != NULL)
	{
		//¾À ¸®½ºÆ® »èÁ¦
		for (SceneMap::iterator iter = sceneMap->begin(); iter != sceneMap->end(); iter++)
		{
			if (iter->second != NULL)
			{
				//¾ÀÀ» ÇÏ³ª¾¿ »èÁ¦
				delete iter->second;
				iter->second = NULL;
			}
		}
		delete sceneMap;
		sceneMap = NULL;
	}

	if (instance != NULL)
	{
		delete instance;
		instance = NULL;
	}
}

SceneMgr* SceneMgr::instance = NULL;
SceneMgr& SceneMgr::Instance()
{
	if (instance == NULL)
	{
		//½Ì±ÛÅæ °´Ã¼¸¦ Ãß°¡
		instance = new SceneMgr();
	}
	return *instance;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : ¸Å´ÏÀú ÃÊ±âÈ­
////////////////////////////////////////////////////////////////////////////////////////////////
void SceneMgr::Init()
{
	AddScene(SCENE(Title, "Title"));
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : ¾ÀÃß°¡
////////////////////////////////////////////////////////////////////////////////////////////////
void SceneMgr::AddScene(std::string sceneName, Scene* scene)
{
	sceneMap->insert({ sceneName, scene });
}
