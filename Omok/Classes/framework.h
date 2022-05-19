#pragma once

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����
#include <windows.h>
// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <time.h>

#include "cocos2d.h"
#include "platform/CCStdC.h"
#include "Manager/SceneMgr/SceneMgr.h"

#define SCENE_MGR SceneMgr::Instance()

#define WINSIZEX 640
#define WINSIZEY 640
