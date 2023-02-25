#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "RSeed.h"

// 모든 연산은 오버플로우를 고려하지 않은 상태임

typedef byte(*calculator)(byte, RSeedData*);
const int CALCULATOR_LEN = 5;

byte calcul1(byte baseData, RSeedData* rSeed) {
	return baseData ^ nextRSeed(rSeed);
}

byte calcul2(byte baseData, RSeedData* rSeed) {
	return mirrorByte(baseData) + nextRSeed(rSeed);
}

byte calcul3(byte baseData, RSeedData* rSeed) {
	byte rseed;
	while ((rseed = nextRSeed(rSeed)) != 0);
	return baseData + pullLeftByte(baseData%rseed) - baseData % rseed;
}

byte calcul4(byte baseData, RSeedData* rSeed) {
	byte rseed;
	while ((rseed = nextRSeed(rSeed)) != 0);
	return pullLeftByte(baseData) + mirrorByte(baseData % rseed);
}

byte calcul5(byte baseData, RSeedData* rSeed) {
	return mirrorByte(pullLeftByte(baseData) ^ nextRSeed(rSeed));
}

calculator calcul[CALCULATOR_LEN] = { calcul1, calcul2, calcul3 , calcul4, calcul5};



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

byte nextJumbledIV(IVData* ivd);
byte getNPreIV(IVData* ivd, int n);

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




void test() {
	byte test1[] = { 0 };
	byte test2[] = { 0 ,0,0,0 };
	byte* test3 = (byte*)malloc(50000*sizeof(byte));

	// all test

	test1[0] = 255;
	for (int i = 0; i < 4; i++)
		test2[i] = 255;
	for (int i = 0; i < 50000; i++)
		test3[i] = 255;

	// all test

	// test3 test
	test3[5549] = 254;
	// test3 test


}

void run(byte* data, int len) {
	IVData* d = newIVData(data, len);
	for (int i = 0; i < 4; i++)
		printb(nextJumbledIV(d));
}

int main() {

	byte test1[] = { 0 };
	byte test2[] = { 0 ,0,0,0 };
	byte* test3 = (byte*)malloc(50000 * sizeof(byte));
	byte* test4 = (byte*)malloc(5000 * sizeof(byte));

	/*run(test1,1);
	printf("\n");
	run(test2, 4);
	printf("\n");
	run(test3, 50000);
	printf("\n\n");*/

	test1[0] = 255;
	for (int i = 0; i < 4; i++)
		test2[i] = 255;
	for (int i = 0; i < 50000; i++)
		test3[i] = 255;
	for (int i = 0; i < 5000; i++)
		test4[i] = 255;


	test3[120] = 254;

	IVData* d[4] = { newIVData(test1,1), newIVData(test2,4), newIVData(test3,50000), newIVData(test4,5000) };
	for (int i = 0; i < 120; i++) {
		printb(nextJumbledIV(d[0]));
		printf(" ");
		printb(nextJumbledIV(d[1]));
		printf(" ");
		printb(nextJumbledIV(d[2]));
		printf(" ");
		printb(nextJumbledIV(d[3]));
		printf(" \n");
	}

	/*run(test3, 50000);
	printf("\n\n");

	test3[5549] = 254;

	run(test3, 50000);
	printf("\n\n");*/

	return 0;
}