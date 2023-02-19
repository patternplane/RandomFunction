#include "util.h"
#include "RSeed.h"
#include <stdlib.h>


RSeedData* defaultRSeedData() {
	RSeedData* a = (RSeedData*)malloc(sizeof(RSeedData));
	a->value1 = 0;
	a->timer1 = 0;
	a->value2 = 0;
	a->timer2 = 0;
	a->value3_1 = 0;
	a->value3_2 = 2;
	a->timer3 = 1;

	return a;
}

const int TIMER1_LEN = 4;
const int VALUE3_1_DELTA = 3;
const int VALUE3_2_DELTA = 2;

RSeedData* makeRSeed(
	byte value1,
	byte timer1,
	byte value2,
	byte timer2,
	byte value3_1,
	byte value3_2,
	byte timer3) {
	RSeedData* a = (RSeedData*)malloc(sizeof(RSeedData));
	a->value1 = value1;
	a->timer1 = (timer1 % TIMER1_LEN);
	a->value2 = value2;
	a->timer2 = !(!timer2);
	a->value3_1 = value3_1;
	a->value3_2 = value3_2;
	a->timer3 = !(!timer3);

	return a;
}

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