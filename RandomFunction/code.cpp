#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "RSeed.h"
#include "IV.h"

// 모든 연산은 오버플로우를 고려하지 않은 상태임





void test() {
	byte test1[] = { 0 };
	byte test2[] = { 0 ,0,0,0 };
	byte* test3 = (byte*)malloc(50000*sizeof(byte));

	// all test

	test1[0] = 255;
	for (int i = 0; i < 4; i++)
		test2[i] = 255;
	for (int i = 0; i < 50000; i++)
		test3[i] = 255;

	// all test

	// test3 test
	test3[5549] = 254;
	// test3 test


}

void run(byte* data, int len) {
	IVData* d = makeIVData(data, len);
	for (int i = 0; i < 4; i++)
		printb(nextJumbledIV(d));
}

int main() {

	byte test1[] = { 0 };
	byte test2[] = { 0 ,0,0,0 };
	byte* test3 = (byte*)malloc(50000 * sizeof(byte));
	byte* test4 = (byte*)malloc(5000 * sizeof(byte));

	/*run(test1,1);
	printf("\n");
	run(test2, 4);
	printf("\n");
	run(test3, 50000);
	printf("\n\n");*/

	test1[0] = 255;
	for (int i = 0; i < 4; i++)
		test2[i] = 255;
	for (int i = 0; i < 50000; i++)
		test3[i] = 255;
	for (int i = 0; i < 5000; i++)
		test4[i] = 255;


	test3[120] = 254;

	short rate[256] = { 0 };

	IVData* d[4] = { makeIVData(test1,1), makeIVData(test2,4), makeIVData(test3,50000), makeIVData(test4,5000) };
	for (int i = 0; i < 1024; i++) {
		byte k = nextJumbledIV(d[1]);
		printb(k);
		rate[k]++;/*
		printf(" ");
		printb(nextJumbledIV(d[1]));
		printf(" ");
		printb(nextJumbledIV(d[2]));
		printf(" ");
		printb(nextJumbledIV(d[3]));
		printf(" \n");*/
	}

	for (int i = 0; i < 256; i++)
		printf("%d\n", rate[i]);

	/*run(test3, 50000);
	printf("\n\n");

	test3[5549] = 254;

	run(test3, 50000);
	printf("\n\n");*/


	return 0;
}