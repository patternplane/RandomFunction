#pragma once
#include "RandData.h"

typedef byte(*calculator)(byte,RandData*);

const int CALCULATOR_LEN = 5;

byte autoPreSelector(RandData* rd);

byte calcul1(byte, RandData* rd);

byte calcul2(byte, RandData* rd);

byte calcul3(byte, RandData* rd);

byte calcul4(byte, RandData* rd);

byte calcul5(byte, RandData* rd);