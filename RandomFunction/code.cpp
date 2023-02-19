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

typedef struct {
	RSeedData* IVRSeed;
	byte seedEx;
	byte* IV;
	int IVLen;
	int IVPos;
	byte* preIVs;
} IVData;

byte nextJumbledIV(IVData* ivd);

IVData* newIVData(byte* IVdata, int len) {
	IVData* newiv = (IVData*)malloc(sizeof(IVData));
	byte* IVCopy = (byte*)malloc(sizeof(byte) * len);
	byte* newPreIVs = (byte*)malloc(sizeof(byte) * len);
	for (int i = 0; i < len; i++)
		newPreIVs[i] = IVCopy[i] = IVdata[i];
	
	newiv->IVRSeed = defaultRSeedData();
	newiv->seedEx = 0;
	newiv->IV = IVCopy;
	newiv->IVLen = len;
	newiv->IVPos = 0;
	newiv->preIVs = newPreIVs;

	for (int i = 0; i < len; i++)
		nextJumbledIV(newiv);

	return newiv;
}

byte getNPreIV(IVData* ivd, int n) {
	if (ivd->IVPos < (n + 1))
		return (ivd->preIVs)[(ivd->IVLen) - 1 - n + (ivd->IVPos)];
	else
		return (ivd->preIVs)[(ivd->IVPos) - 1 - n];
}

byte nextJumbledIV(IVData* ivd) {
	byte newValue =
		slideByte((ivd->IV)[ivd->IVPos] + (ivd->preIVs)[ivd->IVPos], nextRSeed(ivd->IVRSeed))
		+ (
			(
				getNPreIV(ivd, 5)
				+ getNPreIV(ivd, 3)
			)
			^ getNPreIV(ivd, 1)
		)
		- mirrorByte(nextRSeed(ivd->IVRSeed) ^ (ivd->IV)[ivd->IVPos])
		+ slideByte(
			(
				getNPreIV(ivd, 1)
				+ (ivd->IV)[ivd->IVPos]
			)
			^ nextRSeed(ivd->IVRSeed)
			, nextRSeed(ivd->IVRSeed))
		+ pullLeftByte((ivd->preIVs)[ivd->IVPos]);

	ivd->IVPos = (ivd->IVPos + 1) % (ivd->IVLen);
	(ivd->preIVs)[ivd->IVPos] = newValue;
	if (ivd->seedEx == 255)
		ivd->IVRSeed = makeRSeed(
			getNPreIV(ivd, 6),
			getNPreIV(ivd, 5),
			getNPreIV(ivd, 4),
			getNPreIV(ivd, 3),
			getNPreIV(ivd, 2),
			getNPreIV(ivd, 1),
			getNPreIV(ivd, 0));
	ivd->seedEx++;

	return newValue;
}



int main() {

		return 0;
}