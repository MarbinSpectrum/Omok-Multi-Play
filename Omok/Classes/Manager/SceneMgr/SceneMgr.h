#pragma once

#include "../../Scene/Title/Title.h"
#include "../BaseMgr.h"
#include "cocos2d.h"
#include <unordered_map>
#include <string>

#define CSCENE cocos2d::Scene
#define SCENE(x,y) y,x::createScene()

class SceneMgr : BaseMgr
{
	typedef std::unordered_map<std::string, CSCENE*> SceneMap;

public:
	SceneMgr();
	~SceneMgr();

private:
	static SceneMgr* instance;

public:
	static SceneMgr& Instance();

public:
	virtual void	Init() override;
	void			AddScene(std::string sceneName, CSCENE* scene);
	CSCENE*			GetScene(std::string sceneName);
	CSCENE*			GetNowScene();

private:
	CSCENE* nowScene;
	SceneMap* sceneMap;
};

