#include <stdio.h>
#include "util.h"



const int TIMER1_LEN = 4;
const int VALUE3_1_DELTA = 3;
const int VALUE3_2_DELTA = 2;

/// <summary>
/// ±âº»°ª :
/// value1 : 0
/// timer1 : 0
/// value2 : 0
/// timer2 : 0
/// value3_1 : 0
/// value3_2 : 2
/// timer3 : 1
/// </summary>
typedef struct {
	byte value1;
	byte timer1;
	byte value2;
	byte timer2;
	byte value3_1;
	byte value3_2;
	byte timer3;
} RSeedData;

byte nextRSeed(RSeedData* rSeed) {
	byte nextRSeed = rSeed->value1 + rSeed->value2 + (rSeed->timer3 ? rSeed->value3_1 : rSeed->value3_2);

	rSeed->value1 += (64 + (rSeed->timer1 == 3));
	rSeed->timer1 = (rSeed->timer1 + 1) % TIMER1_LEN;

	rSeed->value2 = ((unsigned char)255 - rSeed->value2) + (rSeed->timer2==1);
	rSeed->timer2 = (!rSeed->timer2);

	if (rSeed->timer3)
		rSeed->value3_1 += VALUE3_1_DELTA;
	else
		rSeed->value3_2 += VALUE3_2_DELTA;
	rSeed->timer3 = (!rSeed->timer3);

	return nextRSeed;
}

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