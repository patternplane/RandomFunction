#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "RSeed.h"



typedef byte(*calculator)(byte, RSeedData*);
const int CALCULATOR_LEN = 5;

byte calcul1(byte baseData, RSeedData* rSeed) {
	return baseData ^ nextRSeed(rSeed);
}

byte calcul2(byte baseData, RSeedData* rSeed) {
	return mirrorByte(baseData) + nextRSeed(rSeed);
}

byte calcul3(byte baseData, RSeedData* rSeed) {
	byte rseed = nextRSeed(rSeed);
	return baseData + pullLeftByte(baseData%rseed) - baseData % rseed;
}

byte calcul4(byte baseData, RSeedData* rSeed) {
	return pullLeftByte(baseData) + mirrorByte(baseData % nextRSeed(rSeed));
}

byte calcul5(byte baseData, RSeedData* rSeed) {
	return mirrorByte(pullLeftByte(baseData) ^ nextRSeed(rSeed));
}

calculator calcul[CALCULATOR_LEN] = { calcul1, calcul2, calcul3 , calcul4, calcul5};



// IV : Initial Value

const int PRE_IV_USE_SIZE = 5;

typedef struct {
	RSeedData* IVRSeed;
	byte seedEx;
	byte* IV;
	int IVLen;
	int IVPos;
	byte* preIVs;
	int IVsPos;
} IVData;


IVData* newIVData(byte* IVdata, int len) {
	IVData* newiv = (IVData*)malloc(sizeof(IVData));
	byte* IVCopy = (byte*)malloc(sizeof(byte)*len);
	byte* newPreIVs = (byte*)malloc(sizeof(byte) * PRE_IV_USE_SIZE);
	
	newiv->IVRSeed = defaultRSeedData();
	newiv->seedEx = 0;
	newiv->IV = IVCopy;
	newiv->IVLen = len;
	newiv->IVPos = 0;
	newiv->preIVs = newPreIVs;
	newiv->IVsPos = 0;

	for (int i = 0; i < 1024; i++)
		nextJumbledIV(newiv);

	return newiv;
}

byte nextJumbledIV(IVData* ivd) {
	byte v1 = slideByte((ivd->IV)[ivd->IVPos], nextRSeed(ivd->IVRSeed));
	byte v2 = (calcul[nextRSeed(ivd->IVRSeed) % CALCULATOR_LEN])(v1, ivd->IVRSeed)
		^ ((ivd->preIVs)[ivd->IVsPos]);

	byte newValue = v2 + nextRSeed(ivd->IVRSeed) + ((nextRSeed(ivd->IVRSeed) % 2) ? v1 : (-v1));

	ivd->IVPos = (ivd->IVPos + 1) % (ivd->IVLen);
	(ivd->preIVs)[ivd->IVsPos] = newValue;
	for (int i = 0; i < PRE_IV_USE_SIZE; i++)
		(ivd->preIVs)[((ivd->IVsPos) + i) % PRE_IV_USE_SIZE] += newValue + nextRSeed(ivd->IVRSeed);
	ivd->IVsPos = ((ivd->IVsPos) + 1) % PRE_IV_USE_SIZE;
	if (ivd->seedEx == 255) {
		ivd->IVRSeed = makeRSeed(
			ivd->preIVs[0 % PRE_IV_USE_SIZE],
			ivd->preIVs[1 % PRE_IV_USE_SIZE],
			ivd->preIVs[2 % PRE_IV_USE_SIZE],
			ivd->preIVs[3 % PRE_IV_USE_SIZE],
			ivd->preIVs[4 % PRE_IV_USE_SIZE],
			nextRSeed(ivd->IVRSeed),
			nextRSeed(ivd->IVRSeed));
		ivd->seedEx++;
	}

	return newValue;
}



int main() {

	return 0;
}