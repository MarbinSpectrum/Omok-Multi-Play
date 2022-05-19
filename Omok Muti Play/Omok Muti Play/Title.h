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
	virtual void Create(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam, HINSTANCE hInst) override;
	virtual void Command(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam, HINSTANCE hInst) override;
	virtual void Render(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam, HINSTANCE hInst) override;

private:
	HWND hEdit;
	HWND hBtn;
	TCHAR str[128];
};

