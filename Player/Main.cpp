#include "Main.h"

#define MAX_TITLE_SIZE 100

HINSTANCE g_hInstance;
const TCHAR g_szWindowClass[MAX_TITLE_SIZE] = L"STAR";

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_TIMER:
		break;
	case WM_KEYDOWN:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


int GetWndClassEx(WNDCLASSEX & wcex) {
	memset(&wcex, 0, sizeof(WNDCLASSEX));

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = g_hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = g_szWindowClass;
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex))
		return 0;
	return 1;
}

int  APIENTRY _tWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR lpCmdLine,
	int nShow) {

	MSG msg;
	WNDCLASSEX wcex;
	HWND hWnd;
	g_hInstance = hInstance;

	if (!GetWndClassEx(wcex)) {
		return 0;
	}

	/*hWnd = CreateWindow(g_szWindowClass, L"Window", WS_POPUP,
		0, 0, X_SIZE, Y_SIZE, NULL, NULL, g_hInstance, NULL);*/

	//HRGN hRGN = CreatePolygonRgn(points, 16, WINDING);
	//SetWindowRgn(hWnd, hRGN, TRUE);

	//SetTimer(hWnd, IDT_MOVE_TIMER, DELTA_TIME_IN_MILLISECONDS, NULL);
	ShowWindow(hWnd, nShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}