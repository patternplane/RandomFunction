#include "util.h"
#include <stdio.h>

byte getBit(byte b, byte pos) {
	if (pos < 1 || pos > 8) {
		return 255;
	}

	return (b & (1 << (pos - 1))) > 0;
}

byte mirrorByte(byte b) {
	byte newB = 0;

	for (int i = 0; i < 8; i++)
		newB += ((getBit(b, i + 1)) ? (1 << (7 - i)) : 0);

	return newB;
}

byte slideByte(byte b, byte pos) {
	return (b << (pos % 8)) + (b >> (8 - (pos % 8)));
}

byte pullLeftByte(byte b) {
	int i = 8;
	while (i != 0 && getBit(b, i) != 1)
		i--;

	if (i == 0)
		return 0;
	else
		return (b << (8 - i));
}

void printb(byte b) {
	printf("%d%d%d%d%d%d%d%d"
		, getBit(b, 8)
		, getBit(b, 7)
		, getBit(b, 6)
		, getBit(b, 5)
		, getBit(b, 4)
		, getBit(b, 3)
		, getBit(b, 2)
		, getBit(b, 1));
}