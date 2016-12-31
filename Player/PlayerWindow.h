#pragma once

#include <windows.h>
#include <assert.h>
#include <tchar.h>
#include <vector>
#include "GlobalDefinitions.h"
#include "ShapedButton.h"

#include "KWindow.h"
#include "BitsOperations.h"

using namespace std;

class PlayerWindow : public KWindow
{
private:
	enum cursorState {
		ARROW,
		HAND
	};

	LONG m_bitMaskWidth;
	LONG m_windowHeight;
	LONG m_windowWidth;

	HBITMAP m_hWindowHBitmap;
	HBITMAP m_BackgroundHBitmap;

	HRGN m_hRgn;

	cursorState m_cursorState = ARROW;
	void(*m_mciNotificationHandler)(void);

	vector<ShapedButton> m_buttons = vector<ShapedButton>();


	LRESULT WndProc(HWND hWnd, UINT uMsg,
		WPARAM wParam, LPARAM IParam);

	HRGN CreateRegionByMask(BYTE* pWindowBits, LONG m_windowHeight, LONG m_windowWidth, BYTE maskColor);
	HRGN ExtendRgn(HRGN hRgn, LONG x1, LONG y1, LONG x2, LONG y2);

	void OnMouseMove(int x, int y);
	void OnLButtonDown(int x, int y);
	void OnDraw(HDC hdc);
	//void OnMciNotify(WPARAM wParam, LPARAM lParam);
	void OnCreate();
public:
	bool CreateEx(DWORD dwExStyle,
		LPCTSTR IpszClass, LPCTSTR IpszName, DWORD dwStyle,
		int x, int y, int nWidth, int nHeight, HWND hParent,
		HMENU hMenu, HINSTANCE hlnst);

	//void setMciNotificationHandler(void(*handler)(void));

	void addButton(ShapedButton button);
	PlayerWindow() {}
	PlayerWindow(HBITMAP maskPicture, BYTE maskColor, HBITMAP backgroundPicture);
};

