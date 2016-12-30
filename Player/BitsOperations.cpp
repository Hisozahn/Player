#include "BitsOperations.h"

void BitsOperations::setBitInBitArray(BYTE * pBitArray
	, LONG widthInBytes, LONG heightIdx, LONG widthIdxInBits) {

	LONG ByteWithNeededBit = heightIdx * widthInBytes + widthIdxInBits / 8;
	pBitArray[ByteWithNeededBit] += 1 << (7 - (widthIdxInBits % 8));
}

LONG BitsOperations::getWidthInBytes(LONG widthInBits) {
	return (widthInBits % 16 == 0 ? widthInBits / 16 : widthInBits / 16 + 1) * 2;
}
