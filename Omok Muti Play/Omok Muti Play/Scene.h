#pragma once

#include "resource.h"
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
	virtual void Create(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam, HINSTANCE hInst);
	virtual void Command(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam, HINSTANCE hInst);
	virtual void Render(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam, HINSTANCE hInst);
	std::string	 GetSceneName();

private:
	std::string	 sceneName;
};