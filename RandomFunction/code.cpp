#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "RSeed.h"
#include "IV.h"
#include "RandData.h"

// 모든 연산은 오버플로우를 고려하지 않은 상태임

int main() {

	byte test1[] = { 0 };
	byte test2[] = { 0 ,0,0,0 };
	byte* test3 = (byte*)malloc(50000 * sizeof(byte));
	byte* test4 = (byte*)malloc(5000 * sizeof(byte));

	/*
	test1[0] = 255;
	for (int i = 0; i < 4; i++)
		test2[i] = 255;
	for (int i = 0; i < 50000; i++)
		test3[i] = 255;
	for (int i = 0; i < 5000; i++)
		test4[i] = 255;

	test3[120] = 254;
	*/

	RandData* d[4] = { makeRandData(test1,1), makeRandData(test2,4), makeRandData(test3,50000), makeRandData(test4,5000) };
	for (int i = 0; i < 1024; i++) {
		printb(nextRD(d[0]));
		printf(" ");
		printb(nextRD(d[1]));
		printf(" ");
		printb(nextRD(d[2]));
		printf(" ");
		printb(nextRD(d[3]));
		printf(" \n");
	}

	return 0;
}