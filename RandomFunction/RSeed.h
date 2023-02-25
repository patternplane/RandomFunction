#pragma once

/// <summary>
/// 기본값 :
/// value1 : 0
/// timer1 : 0
/// value2 : 0
/// timer2 : 0
/// value3_1 : 0
/// value3_2 : 2
/// timer3 : 1
/// 
/// 값의 범위 :
/// value1 : 0 ~ 255
/// timer1 : 0 ~ 3
/// value2 : 0 ~ 255
/// timer2 : 0 ~ 1
/// value3_1 : 0 ~ 255
/// value3_2 : 0 ~ 255
/// timer3 : 0 ~ 1
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

RSeedData* defaultRSeedData();

RSeedData* makeRSeed(
	byte value1,
	byte timer1,
	byte value2,
	byte timer2,
	byte value3_1,
	byte value3_2,
	byte timer3);

void setRSeedPreset(
	RSeedData* rseed,
	byte value1,
	byte timer1,
	byte value2,
	byte timer2,
	byte value3_1,
	byte value3_2,
	byte timer3);

byte nextRSeed(RSeedData* rSeed);