#include "PlayerWindow.h"

void PlayerWindow::SetBitMaskBit(BYTE* pBits, LONG widthInBytes, LONG heightIdx, LONG widthIdxInBits) {
	LONG ByteWithNeededBit = heightIdx * widthInBytes + widthIdxInBits / 8;
	pBits[ByteWithNeededBit] += 1 << (7 - (widthIdxInBits % 8));
}

HRGN PlayerWindow::ExtendRgn(HRGN hRgn, int x1, int y1, int x2, int y2) {
	HRGN newHRgn = hRgn;
	if (newHRgn == 0) {
		newHRgn = CreateRectRgn(x1, y1, x2, y2);
	}
	else {
		CombineRgn(newHRgn, newHRgn, CreateRectRgn(x1, y1, x2, y2), RGN_OR);
	}
	return newHRgn;
}

HRGN PlayerWindow::CreateRegionByMask(BYTE* pWindowBits, LONG height, LONG width, BYTE maskColor) {
	HRGN hRgn;
	BYTE pixel;
	LONG xStart = -1;

	for (LONG heightIdx = 0; heightIdx < height; heightIdx++) {
		for (LONG widthIdx = 0; widthIdx < width; widthIdx++) {
			pixel = pWindowBits[heightIdx * width + widthIdx];

			if (pixel != maskColor && xStart == -1) {
				xStart = widthIdx;
			}
			else if (pixel == maskColor && xStart != -1) {
				hRgn = ExtendRgn(hRgn, xStart, height - heightIdx - 1
					, widthIdx, height - heightIdx);
				xStart = -1;
			}
		}
		if (xStart != -1) {
			hRgn = ExtendRgn(hRgn, xStart, height - heightIdx - 1
				, width, height - heightIdx);
			xStart = -1;
		}
	}
	return hRgn;
}

PlayerWindow::PlayerWindow(LPCWSTR shapeMaskPicturePath, BYTE maskColor) {
	BITMAP bm;
	HBITMAP hMask = (HBITMAP)LoadImage(NULL, shapeMaskPicturePath, IMAGE_BITMAP, 0, 0
		, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	assert(hMask != NULL);

	GetObject(hMask, sizeof(bm), &bm);

	m_bitMaskWidth = getMaskWidthFromPixelWidth(bm.bmWidth);

	BYTE* pWindowBits = new unsigned char[bm.bmHeight * m_bitMaskWidth];
	memset(pWindowBits, 0, sizeof(unsigned char) * bm.bmHeight * m_bitMaskWidth);

	m_windowHeight = bm.bmHeight;
	m_windowWidth = bm.bmWidth;

	BYTE* pMaskBits = (BYTE*)bm.bmBits;

	BYTE pixel;
	for (LONG heightIdx = 0; heightIdx < bm.bmHeight; heightIdx++) {
		for (LONG widthIdx = 0; widthIdx < bm.bmWidth; widthIdx++) {
			pixel = pWindowBits[heightIdx * bm.bmWidth + widthIdx];
			if (pixel != maskColor) {
				SetBitMaskBit(pWindowBits, m_bitMaskWidth, bm.bmHeight - heightIdx - 1, widthIdx);
			}
		}
	}
	HRGN hRgn = CreateRegionByMask(pWindowBits, m_windowHeight, m_windowWidth, maskColor);

	m_hBmpWindowMask = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, pWindowBits);

	DeleteObject(hMask);
}

LONG PlayerWindow::getMaskWidthFromPixelWidth(LONG widthInPixels) {
	return (widthInPixels % 16 == 0 ? widthInPixels / 16 : widthInPixels / 16 + 1) * 2;
}

LRESULT PlayerWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	int x = 0;
	int y = 0;
	switch (message)
	{
	case WM_SETCURSOR:
		DoNothing();
		break;
	case WM_MOUSEMOVE:
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		OnMouseMove(x, y);
		break;
	case MM_MCINOTIFY:
		OnMciNotify(wParam, lParam);
		break;
	case WM_LBUTTONDOWN:
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		OnLButtonDown(x, y);
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		BeginPaint(m_hWnd, &ps);
		OnDraw(ps.hdc);
		EndPaint(m_hWnd, &ps);
	}
		break;
	case WM_CREATE:
		OnCreate();
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void PlayerWindow::OnCreate() {
	
}

void PlayerWindow::addButton(ShapedButton button) {
	m_buttons.push(button);
}

void PlayerWindow::OnMouseMove(int x, int y) {
	cursorOnButton = isCursorOnButton(x, y);
	if (cursorOnButton && g_cursorState == ARROW) {
		SetCursor(LoadCursor(NULL, IDC_HAND));
		g_cursorState = HAND;
	}
	else if (!cursorOnButton && g_cursorState == HAND) {
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		g_cursorState = ARROW;
	}
}

