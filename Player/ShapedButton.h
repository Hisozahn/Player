#pragma once

#include <windows.h>
#include <assert.h>
#include <tchar.h>

class ShapedButton
{
private:
	LONG m_bitMaskWidth = 0;
	BYTE* m_pButtonBits = NULL;
	HBITMAP m_hBmpButtonMask = NULL;

	LONG m_parentWindowHeight = 0;
	LONG m_parentWindowWidth = 0;

	HBITMAP m_activePicture = NULL;
	HBITMAP m_disabledPicture = NULL;

	void setBitMaskBit(BYTE* pBits, LONG widthInBytes, LONG heightIdx, LONG widthIdxInBits);

	LONG getMaskWidthFromPixelWidth(LONG widthInPixels);

public:
	ShapedButton();
	ShapedButton(HBITMAP maskPicture, BYTE maskColor, LPCWSTR activePicturePath, LPCWSTR disablePicturePath);
	~ShapedButton();

	BOOL IsCoordinatesInside(LONG x, LONG y) {
		if (m_pButtonBits == NULL || m_bitMaskWidth == 0
			|| y >= m_parentWindowHeight || x >= m_parentWindowWidth
			|| y < 0 || x < 0) {
			return FALSE;
		}
		else {
			return (m_pButtonBits[y * m_bitMaskWidth + x / 8] >> (7 - (x % 8))) % 2 == 0;
		}
	}
};

