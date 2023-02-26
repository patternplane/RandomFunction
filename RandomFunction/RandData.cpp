#include <stdlib.h>
#include "RandData.h"
#include "RandDataCalulator.h"


byte getNPreRD(RandData* rd, int n) {
	n++;
	int pos = (rd->preRDPos - n) % PRE_RD_LEN;
	if (pos < 0)
		pos += PRE_RD_LEN;
	return (rd->preRD)[pos];
}

byte nextRDRSeed(RandData* rd) {
	int index =
		(getNPreRD(rd, 51)
			+ getNPreRD(rd, 125)
			+ getNPreRD(rd, 15)
			+ getNPreIV(rd->iv, 32)
			) % RD_RSEED_COUNT;

	for (int i = 0; i < RD_RSEED_COUNT; i++)
		if (i != index)
			nextRSeed(rd->rSeeds[i]);

	for (int i = 0; i < RD_RSEED_COUNT; i++)
		if (rd->rSeedEXs[i] == 255)
			setRSeedPreset(
				rd->rSeeds[i],
				nextJumbledIV(rd->iv) + nextRDRSeed(rd) - getNPreRD(rd, 5),
				nextJumbledIV(rd->iv) & (nextRDRSeed(rd) | getNPreRD(rd, 4)),
				nextJumbledIV(rd->iv) + nextRDRSeed(rd),
				nextJumbledIV(rd->iv) - nextRDRSeed(rd) + getNPreRD(rd, 5),
				nextJumbledIV(rd->iv) + getNPreIV(rd->iv, 52),
				nextJumbledIV(rd->iv) - getNPreIV(rd->iv, 102) ^ nextRDRSeed(rd),
				nextJumbledIV(rd->iv) | nextRDRSeed(rd));
	(rd->rSeedEXs[index])++;

	return nextRSeed(rd->rSeeds[index]);
}

RandData* makeRandData(byte* seedData, int len) {
	RandData* newRD = (RandData*)malloc(sizeof(RandData));

	newRD->iv = makeIVData(seedData, len);
	for (int i = 0; i < PRE_RD_LEN; i++)
		newRD->preRD[i] = nextJumbledIV(newRD->iv);
	newRD->preRDPos = 0;
	for (int i = 0; i < RD_RSEED_COUNT; i++) {
		newRD->rSeeds[i] = makeRSeed(
			nextJumbledIV(newRD->iv),
			nextJumbledIV(newRD->iv),
			nextJumbledIV(newRD->iv),
			nextJumbledIV(newRD->iv),
			nextJumbledIV(newRD->iv),
			nextJumbledIV(newRD->iv),
			nextJumbledIV(newRD->iv)
		);
		newRD->rSeedEXs[i] = 0;
	}

	for (int k = 0; k < 4; k++) {
		for (int i = 0; i < PRE_RD_LEN; i++)
			nextRD(newRD);
		for (int i = 0; i < RD_RSEED_COUNT; i++)
			setRSeedPreset(
				newRD->rSeeds[i],
				getNPreRD(newRD, 32),
				getNPreRD(newRD, 200),
				getNPreRD(newRD, 50),
				getNPreRD(newRD, 27),
				getNPreRD(newRD, 159),
				getNPreRD(newRD, 305),
				getNPreRD(newRD, 202));
	}

	return newRD;
}

calculator calcul[CALCULATOR_LEN] = { calcul1, calcul2, calcul3, calcul4, calcul5 };

byte calculSelector(RandData* rd) {
	return
		pullRightByte(mirrorByte(
			mirrorByte(
				mirrorByte(
					mirrorByte(
						mirrorByte(getNPreRD(rd, 5) ^ getNPreIV(rd->iv, 227))
						- getNPreRD(rd, 152) - nextJumbledIV(rd->iv))
					+ nextJumbledIV(rd->iv) - getNPreRD(rd, 57))
				+ getNPreIV(rd->iv, 121) + getNPreRD(rd, 23)))
			- nextRDRSeed(rd))
		% CALCULATOR_LEN;
}

const int CALCUL_TURN = 16;

byte applyCalcul(RandData* rd) {
	byte newValue = (calcul[calculSelector(rd)])(getNPreRD(rd,0),rd);
	for (int i = 0; i < CALCUL_TURN - 1; i++)
		newValue = (calcul[calculSelector(rd)])(newValue,rd);
	return newValue;
}

byte nextRD(RandData* rd) {
	byte newValue = applyCalcul(rd);

	(rd->preRD)[rd->preRDPos] = newValue;
	rd->preRDPos = (rd->preRDPos + 1) % PRE_RD_LEN;

	return (calcul[calculSelector(rd)])(newValue, rd);
}