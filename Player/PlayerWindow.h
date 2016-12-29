#pragma once

#include <windows.h>
#include <assert.h>
#include <tchar.h>
#include <stack>
#include "GlobalDefinitions.h"
#include "ShapedButton.h"

#include "KWindow.h"

#define BITMAP_FILE_BACKGROUND L"back.bmp"
#define BITMAP_FILE_ACTIVE_BUTTON L"active.bmp"
#define BITMAP_FILE_DISABLED_BUTTON L"back.bmp"
#define BITMAP_FILE_MASK L"mask.bmp"

using namespace std;

class PlayerWindow : public KWindow
{
private:
	LONG m_bitMaskWidth;
	LONG m_windowHeight;
	LONG m_windowWidth;

	HBITMAP m_hBmpWindowMask;

	stack<ShapedButton> m_buttons = stack<ShapedButton>();

	ShapedButton m_playButton;

	LRESULT WndProc(HWND hWnd, UINT uMsg,
		WPARAM wParam, LPARAM IParam);

	LONG getMaskWidthFromPixelWidth(LONG widthInPixels);

	HRGN CreateRegionByMask(BYTE* pWindowBits, LONG m_windowHeight, LONG m_windowWidth, BYTE maskColor);
	HRGN ExtendRgn(HRGN hRgn, int x1, int y1, int x2, int y2);
	void SetBitMaskBit(BYTE* pBits, LONG widthInBytes, LONG heightIdx, LONG widthIdxInBits);

	void OnMouseMove(int x, int y);
	void OnLButtonDown(int x, int y);

	void OnDraw(HDC hdc);

	void OnMciNotify(WPARAM wParam, LPARAM lParam);

	void OnCreate();
public:
	void addButton(ShapedButton button);
	PlayerWindow();
	PlayerWindow(LPCWSTR shapeMaskPicturePath, BYTE maskColor);
};

