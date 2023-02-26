#pragma once
#include "IV.h"
#include "RandDataCalulator.h"

typedef struct {
	IVData* iv;
	byte preRD[PRE_RD_LEN];
	int preRDPos;
	RSeedData* rSeeds[RD_RSEED_COUNT];
	byte rSeedEXs[RD_RSEED_COUNT];
} RandData;

byte getNPreRD(RandData* rd, int n);

byte nextRDRSeed(RandData* rd);

RandData* makeRandData(byte* seedData, int len);

byte calculSelector(RandData* rd);

byte applyCalcul(RandData* rd);

byte nextRD(RandData* rd);