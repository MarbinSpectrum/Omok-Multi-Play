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
	}
	return *instance;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : �Ŵ��� �ʱ�ȭ
////////////////////////////////////////////////////////////////////////////////////////////////
void SceneMgr::Init()
{
	AddScene(SCENE(Title, "Title"));
	AddScene(SCENE(Lobby, "Lobby"));
	AddScene(SCENE(Room, "Room"));

	nowScene = GetScene("Title");
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : �� �߰�
////////////////////////////////////////////////////////////////////////////////////////////////
void SceneMgr::AddScene(std::string sceneName, CSCENE* scene)
{
	auto director = Director::getInstance();

	sceneMap->insert({ sceneName, scene });
	director->pushScene(scene);
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : �� ��������
////////////////////////////////////////////////////////////////////////////////////////////////
CSCENE* SceneMgr::GetScene(std::string sceneName)
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

	CSCENE* m_scene = iter->second;
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
CSCENE* SceneMgr::GetNowScene()
{
	return nowScene;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : ���̵�
////////////////////////////////////////////////////////////////////////////////////////////////
void SceneMgr::MoveScene(std::string sceneName)
{
	auto scene = GetScene(sceneName);
	MoveScene(scene);
}

void SceneMgr::MoveScene(CSCENE* scene)
{
	auto director = Director::getInstance();

	director->replaceScene(scene);
	nowScene = scene;
	nowScene->Start();
}