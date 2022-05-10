#pragma once

#include "BaseMgr.h"
#include "Scene.h"
#include "Title.h"
#include <string>
#include <unordered_map>
#include <memory>

#define SCENE(x,y) y,new x(y)

class SceneMgr : BaseMgr
{
	typedef std::unordered_map<std::string, Scene*> SceneMap;
	
public:
	SceneMgr();
	~SceneMgr();

private:
	static SceneMgr* instance;

public:
	static SceneMgr& Instance();

public:
	virtual void	Init() override;
	void			AddScene(std::string sceneName, Scene* scene);

private:
	SceneMap*		sceneMap;
};

