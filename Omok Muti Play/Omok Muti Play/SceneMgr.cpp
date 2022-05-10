#include "SceneMgr.h"

SceneMgr::SceneMgr() 
: sceneMap(new SceneMap)
{
	//������
}

SceneMgr::~SceneMgr()
{
	//�Ҹ���

	if (sceneMap != NULL)
	{
		//�� ����Ʈ ����
		for (SceneMap::iterator iter = sceneMap->begin(); iter != sceneMap->end(); iter++)
		{
			if (iter->second != NULL)
			{
				//���� �ϳ��� ����
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
		//�̱��� ��ü�� �߰�
		instance = new SceneMgr();
	}
	return *instance;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : �Ŵ��� �ʱ�ȭ
////////////////////////////////////////////////////////////////////////////////////////////////
void SceneMgr::Init()
{
	AddScene(SCENE(Title, "Title"));
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : ���߰�
////////////////////////////////////////////////////////////////////////////////////////////////
void SceneMgr::AddScene(std::string sceneName, Scene* scene)
{
	sceneMap->insert({ sceneName, scene });
}
