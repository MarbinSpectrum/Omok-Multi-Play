#pragma once

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
// C 런타임 헤더 파일입니다.
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
