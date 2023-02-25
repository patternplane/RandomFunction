#include "IV.h"
#include <stdlib.h>

// IV : Initial Value

IVData* newIVData(byte* IVdata, int len) {
	IVData* newiv = (IVData*)malloc(sizeof(IVData));
	byte* IVCopy = (byte*)malloc(sizeof(byte) * len);
	for (int i = 0; i < len; i++)
		IVCopy[i] = IVdata[i];

	newiv->IVRSeed = defaultRSeedData();
	newiv->seedEx = 0;
	newiv->IV = IVCopy;
	newiv->IVLen = len;
	newiv->IVPos = 0;
	for (int i = 0; i < PRE_IV_LEN; i++)
		newiv->preIVs[i] = IVCopy[i % len] + nextRSeed(newiv->IVRSeed);
	newiv->preIVPos = 0;
	newiv->totalSum = 0;

	int InitTurn = ((len < PRE_IV_LEN) ? (((PRE_IV_LEN / len) + 1) * len) : len);
	for (int k = 0; k < 4; k++) {
		for (int i = 0; i < InitTurn; i++)
			nextJumbledIV(newiv);
		setRSeedPreset(
			newiv->IVRSeed,
			getNPreIV(newiv, 32),
			getNPreIV(newiv, 200),
			getNPreIV(newiv, 50),
			getNPreIV(newiv, 27),
			getNPreIV(newiv, 159),
			getNPreIV(newiv, 305),
			getNPreIV(newiv, 202));
	}

	return newiv;
}

byte getNPreIV(IVData* ivd, int n) {
	n++;
	int pos = (ivd->preIVPos - n) % PRE_IV_LEN;
	if (pos < 0)
		pos += PRE_IV_LEN;
	return (ivd->preIVs)[pos];
}

byte getLastestIV(IVData* ivd) {
	int pos = (ivd->IVPos - 1) % ivd->IVLen;
	if (pos < 0)
		pos += ivd->IVLen;
	return (ivd->IV)[pos];
}

byte IVJumbler(byte baseByte, IVData* ivd) {
	return slideByte(baseByte + getNPreIV(ivd, PRE_IV_LEN - 1), nextRSeed(ivd->IVRSeed))
		+ (
			(
				getNPreIV(ivd, 50)
				+ getNPreIV(ivd, 30)
				)
			^ getNPreIV(ivd, 112)
			)
		- mirrorByte(nextRSeed(ivd->IVRSeed) ^ baseByte)
		+ slideByte(
			(
				getNPreIV(ivd, 135)
				+ baseByte
				)
			^ nextRSeed(ivd->IVRSeed)
			, nextRSeed(ivd->IVRSeed))
		+ pullLeftByte(getNPreIV(ivd, PRE_IV_LEN - 1))
		+ slideByte(mirrorByte(ivd->totalSum), nextRSeed(ivd->IVRSeed));
}

byte nextJumbledIV(IVData* ivd) {
	byte newValue = IVJumbler(getLastestIV(ivd), ivd);

	(ivd->IV)[ivd->IVPos] = newValue;
	ivd->IVPos = (ivd->IVPos + 1) % (ivd->IVLen);
	(ivd->preIVs)[ivd->preIVPos] = newValue;
	ivd->preIVPos = (ivd->preIVPos + 1) % PRE_IV_LEN;
	if (ivd->seedEx == 255)
		setRSeedPreset(
			ivd->IVRSeed,
			getNPreIV(ivd, 6),
			getNPreIV(ivd, 5),
			getNPreIV(ivd, 4),
			getNPreIV(ivd, 3),
			getNPreIV(ivd, 2),
			getNPreIV(ivd, 1),
			getNPreIV(ivd, 0));
	ivd->seedEx++;
	ivd->totalSum += newValue;

	return IVJumbler(newValue, ivd);
}