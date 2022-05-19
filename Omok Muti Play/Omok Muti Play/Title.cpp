#include "Title.h"

Title::Title(std::string name) 
: Scene(name)
{
	//생성자
}

Title::~Title()
{
	//소멸자
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : 씬을 생성시 처리
////////////////////////////////////////////////////////////////////////////////////////////////
void Title::Create(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam, HINSTANCE hInst)
{
	//입력 박스 처리
	hEdit = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 
		(WINSIZEX - 200) / 2, (WINSIZEY - 150) / 2, 200, 25, hWnd, (HMENU)ID_EDIT, hInst, NULL);
	//8글자 제한
	SendMessage(hEdit, EM_LIMITTEXT, (WPARAM)8, 0);

	//입력 확인 박스
	hBtn = CreateWindow(TEXT("button"), TEXT("확인"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		(WINSIZEX - 100) / 2, (WINSIZEY - 50) / 2, 100, 25, hWnd, (HMENU)0, hInst, NULL);
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : 씬 입력관리
////////////////////////////////////////////////////////////////////////////////////////////////
void Title::Command(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam, HINSTANCE hInst)
{
    int wmId = LOWORD(wParam);
    // 메뉴 선택을 구문 분석합니다:
	switch (HIWORD(wParam))
	{
		case EN_CHANGE:
		{
			GetWindowText(hEdit, str, 128);
			SetWindowText(hWnd, str);
		}
		break;
		case EN_MAXTEXT:
		{
			GetWindowText(hEdit, str, 128);
			SetWindowText(hWnd, str);
		}
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : 씬을 랜더링한다.
////////////////////////////////////////////////////////////////////////////////////////////////
void Title::Render(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam, HINSTANCE hInst)
{
	PAINTSTRUCT ps;
	// Device Context : 출력을 위한 모든 데이터를 가지는 구조체
	HDC hdc = BeginPaint(hWnd, &ps);
	// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
	std::wstring wstr = L"닉네임을 입력해주세요 (8글자 제한)";

	// 텍스트를 출력하는 함수.
	TextOut(hdc, (WINSIZEX - wstr.length() * 13)/2, (WINSIZEY - 250) /2, wstr.c_str(), wstr.length());

	EndPaint(hWnd, &ps);
}
