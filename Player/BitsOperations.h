#pragma once

#include <windows.h>
#include <assert.h>
#include <tchar.h>

class BitsOperations
{
public:
	static void setBitInBitArray(BYTE* pBitArray, LONG widthInBytes, LONG heightIdx, LONG widthIdxInBits);
	static LONG getWidthInBytes(LONG widthInBits);
};

