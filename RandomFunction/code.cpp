#include <stdio.h>
#include "util.h"
#include "RSeed.h"



typedef byte(*calculator)(byte, RSeedData*);

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
	byte rseed = nextRSeed(rSeed);
	return pullLeftByte(baseData) + mirrorByte(baseData % rseed);
}

int main() {
	calculator cal[3] = {calcul1, calcul2, calcul3};

	return 0;
}