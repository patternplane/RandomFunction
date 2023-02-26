#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "RandData.h"

// 모든 연산은 오버플로우를 고려하지 않은 상태임

int main() {

	int testSize = 5000;
	byte* test = (byte*)malloc(testSize * sizeof(byte));
	RandData* testRD = makeRandData(test, testSize);

	for (int i = 0; i < 128; i++) {
		printb(nextRD(testRD));
		printf("\n");
	}

	free(test);

	return 0;
}