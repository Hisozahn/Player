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

ShapedButton::ShapedButton(HBITMAP maskPicture, BYTE maskColor, LPCWSTR activePicturePath, LPCWSTR disabledPicturePath) {
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

	

	m_activePicture = (HBITMAP)LoadImage(NULL, activePicturePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	assert(m_activePicture != NULL);
	m_disabledPicture = (HBITMAP)LoadImage(NULL, disabledPicturePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	assert(m_disabledPicture != NULL);

	m_hBmpButtonMask = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, m_pButtonBits);
}

