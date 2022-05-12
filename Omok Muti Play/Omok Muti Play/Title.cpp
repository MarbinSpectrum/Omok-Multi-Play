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
/// : 씬을 랜더링한다.
////////////////////////////////////////////////////////////////////////////////////////////////
void Title::Render(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	// Device Context : 출력을 위한 모든 데이터를 가지는 구조체
	HDC hdc = BeginPaint(hWnd, &ps);
	// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
	std::wstring wstr = L"닉네임을 입력해주세요";

	// 텍스트를 출력하는 함수.
	TextOut(hdc, (WINSIZEX - wstr.length() * 15)/2, (WINSIZEY - 250) /2, wstr.c_str(), wstr.length());

	EndPaint(hWnd, &ps);
}
