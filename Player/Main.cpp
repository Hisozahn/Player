#include "Main.h"

#define MAX_TITLE_SIZE 100
#define BITMAP_FILE_BACKGROUND L"back.bmp"
#define BITMAP_FILE_MASK L"mask.bmp"
#define WINDOW_NAME L"Window"
#define BLUE 252
#define WHITE 255
//#define DEFAULT_WINDOW_HEIGHT 400
//#define DEFAULT_WINDOW_WIDTH 400

HINSTANCE g_hInstance;
const TCHAR g_szWindowClass[MAX_TITLE_SIZE] = L"STAR";
LONG g_windowHeight = 400;
LONG g_windowWidth = 400;
HBITMAP g_hBmpAllMask = NULL;
HBITMAP g_hBmpButtonMask = NULL;
HBITMAP g_hBmpBackground = NULL;
HWND g_hWnd = NULL;

void handleError(LPCWSTR message) {
	MessageBox(g_hWnd, message, L"Error", NULL);
}

void OnDraw(HDC hdc) {
	BITMAP 			bitmap;
	HDC 			hdcMem;
	HGDIOBJ 		oldBitmap;

	if (g_hBmpBackground == NULL) return;

	hdcMem = CreateCompatibleDC(hdc);
	oldBitmap = SelectObject(hdcMem, g_hBmpBackground);

	GetObject(g_hBmpBackground, sizeof(bitmap), &bitmap);

	MaskBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0,0, g_hBmpAllMask, 0, 0, SRCCOPY);

	SelectObject(hdcMem, oldBitmap);
	DeleteDC(hdcMem);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	switch (message)
	{
		
	case WM_TIMER:
		break;
	case WM_KEYDOWN:
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		OnDraw(hdc);
		EndPaint(hWnd, &ps);
		break;

	case WM_CREATE:
		g_hBmpBackground = (HBITMAP)LoadImage(NULL, BITMAP_FILE_BACKGROUND, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		if (g_hBmpBackground == NULL) {
			handleError(L"Background image not loaded");
		}
		break;
	case WM_DESTROY:
		DeleteObject(g_hBmpAllMask);
		DeleteObject(g_hBmpBackground);
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

void setBitMaskBit(BYTE* pAllBits, LONG widthInBytes, LONG heightIdx, LONG widthIdxInBits) {
	LONG ByteWithNeededBit = heightIdx * widthInBytes + widthIdxInBits / 8;
	pAllBits[ByteWithNeededBit] += 1 << (8 - (widthIdxInBits % 8) - 1);
}

HRGN CreateRegionByMask(LPCWSTR path) {
	BITMAP bm;
	HBITMAP hMask = (HBITMAP)LoadImage(NULL, path, IMAGE_BITMAP, 0, 0
		, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	GetObject(hMask, sizeof(bm), &bm);

	int bitMaskWidth = (bm.bmWidth % 16 == 0 ? bm.bmWidth / 16 : bm.bmWidth / 16 + 1) * 2;
	BYTE* pAllBits = new unsigned char[bm.bmHeight * bitMaskWidth];
	BYTE* pButtonBits = new unsigned char[bm.bmHeight * bitMaskWidth];
	memset(pAllBits, 0, sizeof(unsigned char) * bm.bmHeight * bitMaskWidth);
	memset(pButtonBits, 0, sizeof(unsigned char) * bm.bmHeight * bitMaskWidth);

	g_windowHeight = bm.bmHeight;
	g_windowWidth = bm.bmWidth;

	BYTE* pMaskBits = (BYTE*)bm.bmBits;

	HRGN hRgn = 0;
	int pixel;
	int xStart = -1;

	for (LONG heightIdx = 0; heightIdx < bm.bmHeight; heightIdx++) {
		for (LONG widthIdx = 0; widthIdx < bm.bmWidth; widthIdx++) {
			pixel = pMaskBits[heightIdx * bm.bmWidth + widthIdx];
			if (pixel != WHITE) {
				setBitMaskBit(pAllBits, bitMaskWidth, bm.bmHeight - heightIdx - 1, widthIdx);
			}
			if (pixel == BLUE) {
				setBitMaskBit(pButtonBits, bitMaskWidth, bm.bmHeight - heightIdx - 1, widthIdx);
			}

			if (pixel != 255 && xStart == -1) {
				xStart = widthIdx;
			}
			else if (pixel == 255 && xStart != -1) {
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

	g_hBmpAllMask = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, pAllBits);
	g_hBmpButtonMask = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, pButtonBits);

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

	HRGN hRGN = CreateRegionByMask(BITMAP_FILE_MASK);

	hWnd = CreateWindow(g_szWindowClass, WINDOW_NAME, WS_POPUP,
		0, 0, g_windowWidth, g_windowHeight, NULL, NULL, g_hInstance, NULL);

	SetWindowRgn(hWnd, hRGN, TRUE);

	ShowWindow(hWnd, nShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}