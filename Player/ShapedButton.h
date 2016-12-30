#pragma once

#include <windows.h>
#include <assert.h>
#include <tchar.h>

#include "BitsOperations.h"

enum ButtonStatus {
	ACTIVE,
	DISABLED
};

class ShapedButton
{
private:
	ButtonStatus m_status = DISABLED;

	LONG m_xOffset = 0;
	LONG m_yOffset = 0;

	LONG m_bitMaskWidth = 0;
	BYTE* m_pButtonBits = NULL;

	LONG m_height = 0;
	LONG m_width = 0;

	HBITMAP m_hBmpButtonMask = NULL;
	HBITMAP m_activePicture = NULL;
	HBITMAP m_disabledPicture = NULL;

	void(*m_clickHandler)(void);

public:
	ShapedButton() {}
	ShapedButton(HBITMAP maskPicture, BYTE maskColor, HBITMAP activePicture, HBITMAP disablePicture);
	~ShapedButton();

	BOOL IsCoordinatesInside(LONG x, LONG y);

	ButtonStatus getStatus();
	void setStatus(ButtonStatus status);
	void setClickHandler(void(*handler)(void));
	void setOffset(LONG x, LONG y);

	void click();
	void OnDraw(HDC hdcMem, HDC hdcBuffer, LONG windowWidth, LONG windowHieght);
};

