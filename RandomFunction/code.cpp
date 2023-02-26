#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "RSeed.h"
#include "IV.h"

// 모든 연산은 오버플로우를 고려하지 않은 상태임

const int PRE_RD_LEN = 256;
const int RD_RSEED_COUNT = 3;

typedef struct {
	IVData* iv;
	byte preRD[PRE_RD_LEN];
	int preRDPos;
	RSeedData* rSeeds[RD_RSEED_COUNT];
	byte rSeedEXs[RD_RSEED_COUNT];
} RandData;

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

typedef byte(*calculator)(RandData*);
const int CALCULATOR_LEN = 5;

byte preSelector(RandData* rd) {
	return
		mirrorByte(
			pullRightByte(
				nextJumbledIV(rd->iv)
				& getNPreRD(rd, nextJumbledIV(rd->iv) - nextRDRSeed(rd)))
			+ pullLeftByte(
				nextRDRSeed(rd))
			- getNPreIV(rd->iv, nextJumbledIV(rd->iv) - nextRDRSeed(rd)))
		% 256;
}

byte calcul1(RandData* rd) {
	return
		getNPreRD(rd, preSelector(rd))
		^ (
			mirrorByte(
				getNPreIV(rd->iv, preSelector(rd))
				& getNPreRD(rd, preSelector(rd))
			)
			- nextJumbledIV(rd->iv)
			+ pullLeftByte(
				nextRDRSeed(rd)
				^ getNPreIV(rd->iv, preSelector(rd))
			)
			+ pullRightByte(
				nextRDRSeed(rd)
				| getNPreIV(rd->iv, preSelector(rd))
			)
			)
		^ slideByte(
			mirrorByte(
				getNPreIV(rd->iv, preSelector(rd))
			)
			+ nextJumbledIV(rd->iv)
			, nextRDRSeed(rd) + getNPreRD(rd, preSelector(rd))
		)
		+ nextRDRSeed(rd);
}

byte calcul2(RandData* rd) {
	return
		(getNPreRD(rd, preSelector(rd))
			+ nextRDRSeed(rd)
			)
		^ (mirrorByte(
			nextJumbledIV(rd->iv)
		) + pullRightByte(
			nextJumbledIV(rd->iv)
		) - mirrorByte(
			getNPreIV(rd->iv, preSelector(rd))
			^ nextJumbledIV(rd->iv)
			+ nextRDRSeed(rd)
		)
			)
		+ pullLeftByte(
			(
				getNPreRD(rd, preSelector(rd))
				| getNPreIV(rd->iv, preSelector(rd))
				) ^ nextJumbledIV(rd->iv)
		) - pullRightByte(
			nextRDRSeed(rd)
			+ getNPreIV(rd->iv, preSelector(rd))
			+ getNPreIV(rd->iv, preSelector(rd))

		);
}

byte calcul3(RandData* rd) {
	return
		nextJumbledIV(rd->iv)
		^ mirrorByte(
			nextRDRSeed(rd)
			& nextJumbledIV(rd->iv)
			| getNPreIV(rd->iv, preSelector(rd))
			) + pullLeftByte(
				slideByte(
				nextJumbledIV(rd->iv)
				- getNPreRD(rd, preSelector(rd))
				, getNPreIV(rd->iv, preSelector(rd))
			) & slideByte(
				getNPreIV(rd->iv, preSelector(rd))
				^ nextRDRSeed(rd)
				- nextRDRSeed(rd)
				, getNPreIV(rd->iv, preSelector(rd))
			))
		| getNPreRD(rd, preSelector(rd))
		& nextJumbledIV(rd->iv);
}

byte calcul4(RandData* rd) {
	return
		mirrorByte(
			slideByte(
			nextJumbledIV(rd->iv)
			| nextRDRSeed(rd)
			, slideByte(
				getNPreIV(rd->iv, preSelector(rd))
				+ getNPreRD(rd, preSelector(rd))
				, nextRDRSeed(rd)
				- getNPreIV(rd->iv, preSelector(rd))
			) ^ nextRDRSeed(rd)
		)
		) + pullLeftByte(nextRDRSeed(rd)
			& nextJumbledIV(rd->iv)
			+ getNPreRD(rd, preSelector(rd))
			) - pullRightByte(nextJumbledIV(rd->iv)
				^ getNPreIV(rd->iv, preSelector(rd))
				& nextJumbledIV(rd->iv)
				);
}

byte calcul5(RandData* rd) {
	return
		getNPreRD(rd, preSelector(rd))
		^ (
			pullRightByte(
				getNPreIV(rd->iv, preSelector(rd))
				- getNPreRD(rd, preSelector(rd))
			)
			- nextJumbledIV(rd->iv)
			+ pullLeftByte(
				nextRDRSeed(rd)
				+ getNPreIV(rd->iv, preSelector(rd))
			)
			+ mirrorByte(
				nextRDRSeed(rd)
				| getNPreIV(rd->iv, preSelector(rd))
			)
			)
		- slideByte(
			mirrorByte(
				getNPreIV(rd->iv, preSelector(rd))
			)
			+ nextJumbledIV(rd->iv)
			, nextRDRSeed(rd) + getNPreRD(rd, preSelector(rd))
		)
		+ nextRDRSeed(rd);
}

calculator calcul[CALCULATOR_LEN] = { calcul1, calcul2, calcul3, calcul4, calcul5};

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

	return newRD;
}

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
	IVData* d = makeIVData(data, len);
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

	short rate[256] = { 0 };

	IVData* d[4] = { makeIVData(test1,1), makeIVData(test2,4), makeIVData(test3,50000), makeIVData(test4,5000) };
	for (int i = 0; i < 1024; i++) {
		byte k = nextJumbledIV(d[1]);
		printb(k);
		rate[k]++;/*
		printf(" ");
		printb(nextJumbledIV(d[1]));
		printf(" ");
		printb(nextJumbledIV(d[2]));
		printf(" ");
		printb(nextJumbledIV(d[3]));
		printf(" \n");*/
	}

	for (int i = 0; i < 256; i++)
		printf("%d\n", rate[i]);

	/*run(test3, 50000);
	printf("\n\n");

	test3[5549] = 254;

	run(test3, 50000);
	printf("\n\n");*/


	return 0;
}