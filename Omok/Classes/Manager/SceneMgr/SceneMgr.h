#pragma once

#include "../../Scene/Title/Title.h"
#include "../../Scene/Lobby/Lobby.h"
#include "../../Scene/Room/Room.h"

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

public:
	static SceneMgr& Instance();

public:
	virtual void	Init() override;
	void			AddScene(std::string sceneName, CSCENE* scene);
	CSCENE*			GetScene(std::string sceneName);
	CSCENE*			GetNowScene();
	void			MoveScene(std::string sceneName);
	void			MoveScene(CSCENE* scene);

private:
	static SceneMgr* instance;

private:
	CSCENE* nowScene;
	SceneMap* sceneMap;
};

