#pragma once
#include "RSeed.h"
#include "util.h"

// IV : Initial Value

const int PRE_IV_LEN = 256;

typedef struct {
	RSeedData* IVRSeed;
	byte seedEx;
	byte* IV;
	int IVLen;
	int IVPos;
	byte preIVs[PRE_IV_LEN];
	int preIVPos;
	byte totalSum;
} IVData;

IVData* newIVData(byte* IVdata, int len);

byte getNPreIV(IVData* ivd, int n);

byte getLastestIV(IVData* ivd);

byte IVJumbler(byte baseByte, IVData* ivd);

byte nextJumbledIV(IVData* ivd);