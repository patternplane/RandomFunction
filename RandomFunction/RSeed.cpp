#include "util.h"
#include "RSeed.h"


RSeedData defaultRSeedData() {
	return {
		0
		,0
		,0
		,0
		,0
		,2
		,1
	};
}

const int TIMER1_LEN = 4;
const int VALUE3_1_DELTA = 3;
const int VALUE3_2_DELTA = 2;

byte nextRSeed(RSeedData* rSeed) {
	byte nextRSeed = rSeed->value1 + rSeed->value2 + (rSeed->timer3 ? rSeed->value3_1 : rSeed->value3_2);

	rSeed->value1 += (64 + (rSeed->timer1 == 3));
	rSeed->timer1 = (rSeed->timer1 + 1) % TIMER1_LEN;

	rSeed->value2 = ((unsigned char)255 - rSeed->value2) + (rSeed->timer2 == 1);
	rSeed->timer2 = (!rSeed->timer2);

	if (rSeed->timer3)
		rSeed->value3_1 += VALUE3_1_DELTA;
	else
		rSeed->value3_2 += VALUE3_2_DELTA;
	rSeed->timer3 = (!rSeed->timer3);

	return nextRSeed;
}