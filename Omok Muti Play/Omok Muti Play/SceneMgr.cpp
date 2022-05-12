#include "SceneMgr.h"

SceneMgr::SceneMgr() 
: sceneMap(new SceneMap)
, nowScene(NULL)
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

		instance->Init();
	}
	return *instance;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : �Ŵ��� �ʱ�ȭ
////////////////////////////////////////////////////////////////////////////////////////////////
void SceneMgr::Init()
{
	AddScene(SCENE(Title, "Title"));
	
	nowScene = GetScene("Title");
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : �� �߰�
////////////////////////////////////////////////////////////////////////////////////////////////
void SceneMgr::AddScene(std::string sceneName, Scene* scene)
{
	sceneMap->insert({ sceneName, scene });
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : �� ��������
////////////////////////////////////////////////////////////////////////////////////////////////
Scene* SceneMgr::GetScene(std::string sceneName)
{
	if (sceneMap == NULL)
	{
		return NULL;
	}

	SceneMap m_sceneMap = (*sceneMap);

	SceneMap::iterator iter = m_sceneMap.find(sceneName);
	if (iter == m_sceneMap.end())
	{
		//�ش� �̸��� ���� ������
		return NULL;
	}

	Scene* m_scene = iter->second;
	if (m_scene == NULL)
	{
		//���� ������
		return NULL;
	}

	return m_scene;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : ���� ��������
////////////////////////////////////////////////////////////////////////////////////////////////
Scene* SceneMgr::GetNowScene()
{
	return nowScene;
}
