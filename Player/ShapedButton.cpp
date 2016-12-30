#include "ShapedButton.h"

BOOL ShapedButton::IsCoordinatesInside(LONG x, LONG y) {
	LONG realX = x - m_xOffset;
	LONG realY = y - m_yOffset;
	if (m_pButtonBits == NULL || m_bitMaskWidth == 0
		|| realY >= m_height || realX >= m_width
		|| realY < 0 || realX < 0) {
		return FALSE;
	}
	else {
		BYTE byteWithNeededBit = m_pButtonBits[realY * m_bitMaskWidth + realX / 8];
		return (byteWithNeededBit >> (7 - (x % 8))) % 2 == 0;
	}
}

ButtonStatus ShapedButton::getStatus() {
	return m_status;
}

void ShapedButton::setStatus(ButtonStatus status) {
	m_status = status;
}

void ShapedButton::setClickHandler(void(*handler)(void)) {
	m_clickHandler = handler;
}

void ShapedButton::setOffset(LONG x, LONG y) {
	m_xOffset = x;
	m_yOffset = y;
}

void ShapedButton::click() {
	if (m_clickHandler == NULL) {
		return;
	}
	m_clickHandler();
}

void ShapedButton::OnDraw(HDC hdcMem, HDC hdcBuffer, LONG windowWidth, LONG windowHieght) {
	if (m_status == ACTIVE) {
		SelectObject(hdcMem, m_activePicture);
		BitBlt(hdcBuffer, m_xOffset, m_yOffset, windowWidth, windowHieght, hdcMem, 0, 0, SRCINVERT);
		SelectObject(hdcMem, m_hBmpButtonMask);
		BitBlt(hdcBuffer, m_xOffset, m_yOffset, windowWidth, windowHieght, hdcMem, 0, 0, SRCAND);
		SelectObject(hdcMem, m_activePicture);
		BitBlt(hdcBuffer, m_xOffset, m_yOffset, windowWidth, windowHieght, hdcMem, 0, 0, SRCINVERT);
	}
	else if (m_status == DISABLED) {
		SelectObject(hdcMem, m_disabledPicture);
		BitBlt(hdcBuffer, m_xOffset, m_yOffset, windowWidth, windowHieght, hdcMem, 0, 0, SRCINVERT);
		SelectObject(hdcMem, m_hBmpButtonMask);
		BitBlt(hdcBuffer, m_xOffset, m_yOffset, windowWidth, windowHieght, hdcMem, 0, 0, SRCAND);
		SelectObject(hdcMem, m_disabledPicture);
		BitBlt(hdcBuffer, m_xOffset, m_yOffset, windowWidth, windowHieght, hdcMem, 0, 0, SRCINVERT);
	}
}

ShapedButton::ShapedButton(HBITMAP maskPicture, BYTE maskColor, HBITMAP activePicture, HBITMAP disabledPicture) {
	BITMAP bm;

	assert(maskPicture != NULL);

	GetObject(maskPicture, sizeof(bm), &bm);

	m_height = bm.bmHeight;
	m_width = bm.bmWidth;

	m_bitMaskWidth = BitsOperations::getWidthInBytes(bm.bmWidth);
	m_pButtonBits = new unsigned char[bm.bmHeight * m_bitMaskWidth];
	memset(m_pButtonBits, 0, sizeof(unsigned char) * bm.bmHeight * m_bitMaskWidth);

	BYTE* pMaskBits = (BYTE*)bm.bmBits;

	BYTE pixel;
	LONG xStart = -1;

	for (LONG heightIdx = 0; heightIdx < bm.bmHeight; heightIdx++) {
		for (LONG widthIdx = 0; widthIdx < bm.bmWidth; widthIdx++) {

			pixel = pMaskBits[heightIdx * bm.bmWidth + widthIdx];

			if (pixel != maskColor) {
				BitsOperations::setBitInBitArray(m_pButtonBits, m_bitMaskWidth, bm.bmHeight - heightIdx - 1, widthIdx);
			}
		}
	}

	m_activePicture = activePicture;
	m_disabledPicture = disabledPicture;

	m_hBmpButtonMask = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, m_pButtonBits);
}



ShapedButton::~ShapedButton() {
	//TODO
}

