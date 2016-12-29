#include "ShapedButton.h"

ShapedButton::~ShapedButton()
{
}

void ShapedButton::setBitMaskBit(BYTE* pBits, LONG widthInBytes, LONG heightIdx, LONG widthIdxInBits) {
	LONG ByteWithNeededBit = heightIdx * widthInBytes + widthIdxInBits / 8;
	pBits[ByteWithNeededBit] += 1 << (7 - (widthIdxInBits % 8));
}

LONG ShapedButton::getMaskWidthFromPixelWidth(LONG widthInPixels) {
	return (widthInPixels % 16 == 0 ? widthInPixels / 16 : widthInPixels / 16 + 1) * 2;
}

BOOL ShapedButton::IsCoordinatesInside(LONG x, LONG y) {
	if (m_pButtonBits == NULL || m_bitMaskWidth == 0
		|| y >= m_parentWindowHeight || x >= m_parentWindowWidth
		|| y < 0 || x < 0) {
		return FALSE;
	}
	else {
		return (m_pButtonBits[y * m_bitMaskWidth + x / 8] >> (7 - (x % 8))) % 2 == 0;
	}
}

ButtonStatus ShapedButton::getStatus() {
	return m_status;
}

void ShapedButton::setStatus(ButtonStatus status) {
	m_status = status;
}

void ShapedButton::OnDraw(HDC hdcMem, HDC hdcBuffer) {
	if (m_status == ACTIVE) {
		SelectObject(hdcMem, m_activePicture);
		BitBlt(hdcBuffer, 0, 0, m_parentWindowWidth, m_parentWindowHeight, hdcMem, 0, 0, SRCINVERT);
		SelectObject(hdcMem, m_hBmpButtonMask);
		BitBlt(hdcBuffer, 0, 0, m_parentWindowWidth, m_parentWindowHeight, hdcMem, 0, 0, SRCAND);
		SelectObject(hdcMem, m_activePicture);
		BitBlt(hdcBuffer, 0, 0, m_parentWindowWidth, m_parentWindowHeight, hdcMem, 0, 0, SRCINVERT);
	}
	else if (m_status == DISABLED) {
		SelectObject(hdcMem, m_disabledPicture);
		BitBlt(hdcBuffer, 0, 0, m_parentWindowWidth, m_parentWindowHeight, hdcMem, 0, 0, SRCINVERT);
		SelectObject(hdcMem, m_hBmpButtonMask);
		BitBlt(hdcBuffer, 0, 0, m_parentWindowWidth, m_parentWindowHeight, hdcMem, 0, 0, SRCAND);
		SelectObject(hdcMem, m_disabledPicture);
		BitBlt(hdcBuffer, 0, 0, m_parentWindowWidth, m_parentWindowHeight, hdcMem, 0, 0, SRCINVERT);
	}
}

ShapedButton::ShapedButton(HBITMAP maskPicture, BYTE maskColor, HBITMAP activePicture, HBITMAP disabledPicture) {
	BITMAP bm;

	assert(maskPicture != NULL);

	GetObject(maskPicture, sizeof(bm), &bm);

	m_parentWindowHeight = bm.bmHeight;
	m_parentWindowWidth = bm.bmWidth;

	m_bitMaskWidth = getMaskWidthFromPixelWidth(bm.bmWidth);
	m_pButtonBits = new unsigned char[bm.bmHeight * m_bitMaskWidth];
	memset(m_pButtonBits, 0, sizeof(unsigned char) * bm.bmHeight * m_bitMaskWidth);

	BYTE* pMaskBits = (BYTE*)bm.bmBits;

	BYTE pixel;
	LONG xStart = -1;

	for (LONG heightIdx = 0; heightIdx < bm.bmHeight; heightIdx++) {
		for (LONG widthIdx = 0; widthIdx < bm.bmWidth; widthIdx++) {

			pixel = pMaskBits[heightIdx * bm.bmWidth + widthIdx];

			if (pixel != maskColor) {
				setBitMaskBit(m_pButtonBits, m_bitMaskWidth, bm.bmHeight - heightIdx - 1, widthIdx);
			}
		}
	}

	m_activePicture = activePicture;
	m_disabledPicture = disabledPicture;

	m_hBmpButtonMask = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, m_pButtonBits);
}

