#pragma once

#include <windows.h>
#include <assert.h>
#include <tchar.h>

enum ButtonStatus {
	ACTIVE,
	DISABLED
};

class ShapedButton
{
private:
	ButtonStatus m_status = DISABLED;

	LONG m_bitMaskWidth = 0;
	BYTE* m_pButtonBits = NULL;

	LONG m_parentWindowHeight = 0;
	LONG m_parentWindowWidth = 0;

	HBITMAP m_hBmpButtonMask = NULL;
	HBITMAP m_activePicture = NULL;
	HBITMAP m_disabledPicture = NULL;


	void setBitMaskBit(BYTE* pBits, LONG widthInBytes, LONG heightIdx, LONG widthIdxInBits);

	LONG getMaskWidthFromPixelWidth(LONG widthInPixels);

public:
	ShapedButton() {}
	ShapedButton(HBITMAP maskPicture, BYTE maskColor, HBITMAP activePicture, HBITMAP disablePicture);
	~ShapedButton();

	BOOL IsCoordinatesInside(LONG x, LONG y);

	ButtonStatus getStatus();
	void setStatus(ButtonStatus status);

	void OnDraw(HDC hdcMem, HDC hdcBuffer);
};

