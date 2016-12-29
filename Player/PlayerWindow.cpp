#include "PlayerWindow.h"

void PlayerWindow::SetBitMaskBit(BYTE* pBits, LONG widthInBytes, LONG heightIdx, LONG widthIdxInBits) {
	LONG ByteWithNeededBit = heightIdx * widthInBytes + widthIdxInBits / 8;
	pBits[ByteWithNeededBit] += 1 << (7 - (widthIdxInBits % 8));
}

HRGN PlayerWindow::ExtendRgn(HRGN hRgn, LONG x1, LONG y1, LONG x2, LONG y2) {
	HRGN newHRgn = hRgn;
	if (newHRgn == NULL) {
		newHRgn = CreateRectRgn(x1, y1, x2, y2);
	}
	else {
		CombineRgn(newHRgn, newHRgn, CreateRectRgn(x1, y1, x2, y2), RGN_OR);
	}
	return newHRgn;
}

HRGN PlayerWindow::CreateRegionByMask(BYTE* pWindowBits, LONG height, LONG width, BYTE maskColor) {
	HRGN hRgn = NULL;
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

PlayerWindow::PlayerWindow(HBITMAP maskPicture, BYTE maskColor, HBITMAP backgroundPicture) {
	BITMAP bm;

	GetObject(maskPicture, sizeof(bm), &bm);
	assert(bm.bmBits != NULL);

	m_bitMaskWidth = getMaskWidthFromPixelWidth(bm.bmWidth);

	BYTE* pWindowBits = new unsigned char[bm.bmHeight * m_bitMaskWidth];
	memset(pWindowBits, 0, sizeof(unsigned char) * bm.bmHeight * m_bitMaskWidth);

	m_windowHeight = bm.bmHeight;
	m_windowWidth = bm.bmWidth;

	BYTE* pMaskBits = (BYTE*)bm.bmBits;

	BYTE pixel;
	for (LONG heightIdx = 0; heightIdx < bm.bmHeight; heightIdx++) {
		for (LONG widthIdx = 0; widthIdx < bm.bmWidth; widthIdx++) {
			pixel = pMaskBits[heightIdx * bm.bmWidth + widthIdx];
			if (pixel != maskColor) {
				SetBitMaskBit(pWindowBits, m_bitMaskWidth, bm.bmHeight - heightIdx - 1, widthIdx);
			}
		}
	}
	m_hRgn = CreateRegionByMask(pMaskBits, m_windowHeight, m_windowWidth, maskColor);

	m_hWindowHBitmap = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, pWindowBits);
	m_BackgroundHBitmap = backgroundPicture;
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
	//TODO
}

bool PlayerWindow::CreateEx(DWORD dwExStyle, LPCTSTR IpszClass
	, LPCTSTR IpszName, DWORD dwStyle, int x, int y, int nWidth
	, int nHeight, HWND hParent, HMENU hMenu, HINSTANCE hlnst)
{
	if (!KWindow::CreateEx(dwExStyle, IpszClass
		, IpszName, dwStyle, x, y, nWidth
		, nHeight, hParent, hMenu, hlnst)) {
		return FALSE;
	}
	
	SetWindowRgn(m_hWnd, m_hRgn, TRUE);
	return TRUE;
}

void PlayerWindow::addButton(ShapedButton button) {
	m_buttons.push_back(button);
}

void PlayerWindow::OnMouseMove(int x, int y) {
	BOOL cursorOnButton = FALSE;

	vector<ShapedButton>::iterator it;
	for (it = m_buttons.begin(); it != m_buttons.end(); ++it) {
		ShapedButton button = *it;
		if (button.IsCoordinatesInside(x, y)) {
			cursorOnButton = TRUE;
			break;
		}
	}
	if (cursorOnButton) {
		if (m_cursorState == ARROW) {
			SetCursor(LoadCursor(NULL, IDC_HAND));
			m_cursorState = HAND;
		}
	}
	else if (m_cursorState == HAND) {
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		m_cursorState = ARROW;
	}
}

void PlayerWindow::OnLButtonDown(int x, int y) {
	vector<ShapedButton>::iterator buttonIterator;
	for (buttonIterator = m_buttons.begin(); buttonIterator != m_buttons.end(); ++buttonIterator) {
		if (buttonIterator->IsCoordinatesInside(x, y)) {
			if (buttonIterator->getStatus() == DISABLED) {
				buttonIterator->setStatus(ACTIVE);
			}
			else {
				buttonIterator->setStatus(DISABLED);
			}
			break;
		}
	}
	InvalidateRect(m_hWnd, NULL, FALSE);
	//MessageBox(m_hWnd, _T("Õ¿∆¿À"), _T("Œ“∆¿À"), NULL);
}

void PlayerWindow::OnDraw(HDC hdc) {
	if (m_BackgroundHBitmap == NULL) {
		return;
	}

	BITMAP bitmap;
	GetObject(m_BackgroundHBitmap, sizeof(bitmap), &bitmap);

	HDC hdcMem = CreateCompatibleDC(hdc);
	HGDIOBJ oldBitmap = SelectObject(hdcMem, m_BackgroundHBitmap);

	HDC hdcBuffer = CreateCompatibleDC(hdc);
	HBITMAP bufferBitmap = CreateCompatibleBitmap(hdc, bitmap.bmWidth, bitmap.bmHeight);
	SelectObject(hdcBuffer, bufferBitmap);

	MaskBlt(hdcBuffer, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, m_hWindowHBitmap, 0, 0, SRCCOPY);

	vector<ShapedButton>::iterator it;
	for (it = m_buttons.begin(); it != m_buttons.end(); ++it) {
		it->OnDraw(hdcMem, hdcBuffer);
	}

	BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcBuffer, 0, 0, SRCCOPY);

	SelectObject(hdcMem, oldBitmap);
	DeleteDC(hdcBuffer);
	DeleteObject(bufferBitmap);
}

void PlayerWindow::OnMciNotify(WPARAM wParam, LPARAM lParam) {
	//TODO
}

