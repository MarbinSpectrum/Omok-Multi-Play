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

#include "cocos2d.h"
#include "platform/CCStdC.h"
#include "Manager/SceneMgr/SceneMgr.h"
#include "Manager/MassageMgr/MassageMgr.h"

#define SCENE_MGR SceneMgr::Instance()
#define MASSAGE_MGR MassageMgr::Instance()

#define WINSIZEX 480
#define WINSIZEY 320
