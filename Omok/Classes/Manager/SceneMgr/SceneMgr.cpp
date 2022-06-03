#include "SceneMgr.h"

SceneMgr::SceneMgr()
: sceneMap(new SceneMap)
, nowScene(NULL)
{
	//생성자
}

SceneMgr::~SceneMgr()
{
	//소멸자
	if (sceneMap != NULL)
	{
		delete sceneMap;
		sceneMap = NULL;
	}
}

SceneMgr* SceneMgr::instance = NULL;
SceneMgr& SceneMgr::Instance()
{
	if (instance == NULL)
	{
		//싱글톤 객체를 추가
		instance = new SceneMgr();
	}
	return *instance;
}

void SceneMgr::Destroy()
{
	delete instance;
	instance = NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : 매니저 초기화
////////////////////////////////////////////////////////////////////////////////////////////////
void SceneMgr::Init()
{
	AddScene(SCENE(Title, "Title"));
	AddScene(SCENE(Lobby, "Lobby"));
	AddScene(SCENE(Room, "Room"));
	AddScene(SCENE(InGame, "InGame"));
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : 씬 추가
////////////////////////////////////////////////////////////////////////////////////////////////
void SceneMgr::AddScene(std::string sceneName, CSCENE* scene)
{
	auto director = Director::getInstance();

	sceneMap->insert({ sceneName, scene });
	director->pushScene(scene);
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : 씬 가져오기
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
		//해당 이름의 씬이 없을때
		return NULL;
	}

	CSCENE* m_scene = iter->second;
	if (m_scene == NULL)
	{
		//씬이 없을때
		return NULL;
	}

	return m_scene;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : 현재 가져오기
////////////////////////////////////////////////////////////////////////////////////////////////
CSCENE* SceneMgr::GetNowScene()
{
	return nowScene;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : 씬이동
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