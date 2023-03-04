#pragma once
#include "IV.h"

const int PRE_RD_LEN = 256;
const int RD_RSEED_COUNT = 3;
const int RD_USE_LIMIT = 32;

typedef struct {
	IVData* iv;
	byte preRD[PRE_RD_LEN];
	int preRDPos;
	RSeedData* rSeeds[RD_RSEED_COUNT];
	byte rSeedEXs[RD_RSEED_COUNT];
	int rdUseCount;
	byte currentRDState;
} RandData;

byte getNPreRD(RandData* rd, int n);

byte nextRDRSeed(RandData* rd);

RandData* makeRandData(byte* seedData, int len);

byte calculSelector(RandData* rd);

byte applyCalcul(RandData* rd);

byte RDJumbler(RandData* rd);

byte nextRD(RandData* rd);