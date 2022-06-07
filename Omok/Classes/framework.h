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
#include <vector>

#include "cocos2d.h"
#include "platform/CCStdC.h"
#include "../Algorithm/Algorithm.h"

#include "Manager/SceneMgr/SceneMgr.h"
#include "Manager/MessageMgr/MessageMgr.h"
#include "Manager/PlayerMgr/PlayerMgr.h"

#define SCENE_MGR SceneMgr::Instance()
#define MESSAGE_MGR MessageMgr::Instance()
#define PLAYER_MGR PlayerMgr::Instance()

#define WINSIZEX 480
#define WINSIZEY 320

#define STRINGDATA "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890-=,./?<>;:'[]{}!@#$%^&*()_+"
