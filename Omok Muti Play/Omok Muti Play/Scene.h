#pragma once

#include <windows.h>
#include <string>

////////////////////////////////////////////////////////////////////////////////////////////////
/// : æ¿ ∫£¿ÃΩ∫
////////////////////////////////////////////////////////////////////////////////////////////////
class Scene
{
public:
	Scene(std::string name);
	virtual ~Scene();

public:
	virtual void Render(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
	std::string	 GetSceneName();

private:
	std::string	 sceneName;
};