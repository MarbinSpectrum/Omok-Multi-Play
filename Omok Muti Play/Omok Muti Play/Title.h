#pragma once

#include "Scene.h"
#include "framework.h"
#include <windows.h>
#include <string>

class Title : public Scene
{
public:
	Title(std::string name);
	virtual ~Title();

public:
	virtual void Render(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) override;
};

