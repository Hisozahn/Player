#include "Main.h"

#define MAX_TITLE_SIZE 100
#define DEFAULT_WINDOW_HEIGHT 400
#define DEFAULT_WINDOW_WIDTH 400

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

HRGN ExtendRgn(HRGN hRgn, int x1, int y1, int x2, int y2) {
	HRGN newHRgn = hRgn;
	if (newHRgn == 0) {
		newHRgn = CreateRectRgn(x1, y1, x2, y2);
	}
	else {
		CombineRgn(newHRgn, newHRgn, CreateRectRgn(x1, y1, x2, y2), RGN_OR);
	}
	return newHRgn;
}

HRGN CreateRegionByMask(LPCWSTR path) {
	BITMAP bm;
	HBITMAP hMask = (HBITMAP)LoadImage(NULL, path, IMAGE_BITMAP, 0, 0
		, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	GetObject(hMask, sizeof(bm), &bm);

	BYTE *pMaskBits = (BYTE *)bm.bmBits;

	HRGN hRgn = 0;
	int pixel;
	int xStart = -1;

	for (LONG heightIdx = 0; heightIdx < bm.bmHeight; heightIdx++) {
		for (LONG widthIdx = 0; widthIdx < bm.bmWidth; widthIdx++) {
			pixel = pMaskBits[heightIdx * bm.bmWidth + widthIdx];

			if (pixel != 0 && xStart == -1) {
				xStart = widthIdx;
			}
			else if (pixel == 0 && xStart != -1) {
				hRgn = ExtendRgn(hRgn, xStart, bm.bmHeight - heightIdx - 1
					, widthIdx, bm.bmHeight - heightIdx);
				xStart = -1;
			}
		}
		if (xStart != -1) {
			hRgn = ExtendRgn(hRgn, xStart, bm.bmHeight - heightIdx - 1
				, bm.bmWidth, bm.bmHeight - heightIdx);
			xStart = -1;
		}
	}

	DeleteObject(hMask);

	return hRgn;
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

	hWnd = CreateWindow(g_szWindowClass, L"Window", WS_POPUP,
		0, 0, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, NULL, NULL, g_hInstance, NULL);

	HRGN hRGN = CreateRegionByMask(L"mask.bmp");
	SetWindowRgn(hWnd, hRGN, TRUE);

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