#include "Title.h"

Title::Title(std::string name) 
: Scene(name)
{
	//������
}

Title::~Title()
{
	//�Ҹ���
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : ���� �������Ѵ�.
////////////////////////////////////////////////////////////////////////////////////////////////
void Title::Render(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	// Device Context : ����� ���� ��� �����͸� ������ ����ü
	HDC hdc = BeginPaint(hWnd, &ps);
	// TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�...
	std::wstring wstr = L"�г����� �Է����ּ���";

	// �ؽ�Ʈ�� ����ϴ� �Լ�.
	TextOut(hdc, (WINSIZEX - wstr.length() * 15)/2, (WINSIZEY - 250) /2, wstr.c_str(), wstr.length());

	EndPaint(hWnd, &ps);
}
