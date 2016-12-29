#pragma once
#define STRICT
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <assert.h>
#include <tchar.h>

class KWindow
{
	static LRESULT CALLBACK WindowProc(HWND hWnd,
		UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	bool RegisterClass(LPCTSTR IpszClass, HINSTANCE hlnst);
	virtual void OnDraw(HDC hDC) {}

	virtual void OnKeyDown(WPARAM wParam, LPARAM lParam) {}

	virtual LRESULT WndProc(HWND hWnd, UINT uMsg,
		WPARAM wParam, LPARAM lParam);


	virtual void GetWndClassEx(WNDCLASSEX & vie);
public:

	HWND m_hWnd;

	KWindow(void)
	{
		m_hWnd = NULL;
	}
	virtual ~KWindow(void) {}

	virtual bool CreateEx(DWORD dwExStyle,
		LPCTSTR IpszClass, LPCTSTR IpszName, DWORD dwStyle,
		int x, int y, int nWidth, int nHeight, HWND hParent,
		HMENU hMenu, HINSTANCE hlnst);


	virtual WPARAM MessageLoop(void);

	BOOL ShowWindow(int nCmdShow) const
	{
		return ::ShowWindow(m_hWnd, nCmdShow);
	}

	BOOL UpdateWindow(void) const
	{
		return ::UpdateWindow(m_hWnd);
	}
};