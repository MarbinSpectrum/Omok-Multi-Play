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
/// : ���� ������ ó��
////////////////////////////////////////////////////////////////////////////////////////////////
void Title::Create(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam, HINSTANCE hInst)
{
	//�Է� �ڽ� ó��
	hEdit = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 
		(WINSIZEX - 200) / 2, (WINSIZEY - 150) / 2, 200, 25, hWnd, (HMENU)ID_EDIT, hInst, NULL);
	//8���� ����
	SendMessage(hEdit, EM_LIMITTEXT, (WPARAM)8, 0);

	//�Է� Ȯ�� �ڽ�
	hBtn = CreateWindow(TEXT("button"), TEXT("Ȯ��"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		(WINSIZEX - 100) / 2, (WINSIZEY - 50) / 2, 100, 25, hWnd, (HMENU)0, hInst, NULL);
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : �� �Է°���
////////////////////////////////////////////////////////////////////////////////////////////////
void Title::Command(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam, HINSTANCE hInst)
{
    int wmId = LOWORD(wParam);
    // �޴� ������ ���� �м��մϴ�:
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
/// : ���� �������Ѵ�.
////////////////////////////////////////////////////////////////////////////////////////////////
void Title::Render(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam, HINSTANCE hInst)
{
	PAINTSTRUCT ps;
	// Device Context : ����� ���� ��� �����͸� ������ ����ü
	HDC hdc = BeginPaint(hWnd, &ps);
	// TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�...
	std::wstring wstr = L"�г����� �Է����ּ��� (8���� ����)";

	// �ؽ�Ʈ�� ����ϴ� �Լ�.
	TextOut(hdc, (WINSIZEX - wstr.length() * 13)/2, (WINSIZEY - 250) /2, wstr.c_str(), wstr.length());

	EndPaint(hWnd, &ps);
}
