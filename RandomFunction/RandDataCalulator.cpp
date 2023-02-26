#include "RandDataCalulator.h"


byte autoPreSelector(RandData* rd) {
	return
		mirrorByte(
			pullRightByte(
				nextJumbledIV(rd->iv)
				& getNPreRD(rd, nextJumbledIV(rd->iv) - nextRDRSeed(rd)))
			+ pullLeftByte(
				nextRDRSeed(rd))
			- getNPreIV(rd->iv, nextJumbledIV(rd->iv) - nextRDRSeed(rd)))
		% 256;
}

byte calcul1(byte baseData, RandData* rd) {
	return
		baseData
		+ getNPreRD(rd, autoPreSelector(rd))
		^ (
			mirrorByte(
				getNPreIV(rd->iv, autoPreSelector(rd))
				& getNPreRD(rd, autoPreSelector(rd))
			)
			- nextJumbledIV(rd->iv)
			+ pullLeftByte(
				nextRDRSeed(rd)
				- baseData
			)
			+ pullRightByte(
				nextRDRSeed(rd)
				| getNPreIV(rd->iv, autoPreSelector(rd))
			)
			)
		^ slideByte(
			mirrorByte(
				getNPreIV(rd->iv, autoPreSelector(rd))
			)
			+ nextJumbledIV(rd->iv)
			, nextRDRSeed(rd) + getNPreRD(rd, autoPreSelector(rd))
		)
		+ nextRDRSeed(rd);
}

byte calcul2(byte baseData, RandData* rd) {
	return
		baseData
		^ (getNPreRD(rd, autoPreSelector(rd))
			+ nextRDRSeed(rd)
			)
		^ (mirrorByte(
			nextJumbledIV(rd->iv)
		) + pullRightByte(
			nextJumbledIV(rd->iv)
		) - mirrorByte(
			getNPreIV(rd->iv, autoPreSelector(rd))
			^ nextJumbledIV(rd->iv)
			+ nextRDRSeed(rd)
		)
			)
		+ pullLeftByte(
			(
				getNPreRD(rd, autoPreSelector(rd))
				| getNPreIV(rd->iv, autoPreSelector(rd))
				) ^ nextJumbledIV(rd->iv)
		) - pullRightByte(
			nextRDRSeed(rd)
			+ baseData
			+ getNPreIV(rd->iv, autoPreSelector(rd))

		);
}

byte calcul3(byte baseData, RandData* rd) {
	return
		baseData
		+ nextJumbledIV(rd->iv)
		^ mirrorByte(
			nextRDRSeed(rd)
			& nextJumbledIV(rd->iv)
			| getNPreIV(rd->iv, autoPreSelector(rd))
		) + pullLeftByte(
			slideByte(
				nextJumbledIV(rd->iv)
				- getNPreRD(rd, autoPreSelector(rd))
				, getNPreIV(rd->iv, autoPreSelector(rd))
			) & slideByte(
				baseData
				^ nextRDRSeed(rd)
				- nextRDRSeed(rd)
				, getNPreIV(rd->iv, autoPreSelector(rd))
			))
		| getNPreRD(rd, autoPreSelector(rd))
		& nextJumbledIV(rd->iv);
}

byte calcul4(byte baseData, RandData* rd) {
	return
		mirrorByte(
			slideByte(
				nextJumbledIV(rd->iv)
				| nextRDRSeed(rd)
				+ baseData
				, slideByte(
					getNPreIV(rd->iv, autoPreSelector(rd))
					+ getNPreRD(rd, autoPreSelector(rd))
					, nextRDRSeed(rd)
					- baseData
				) ^ (nextRDRSeed(rd)
					- baseData)
			)
		) + pullLeftByte(nextRDRSeed(rd)
			& nextJumbledIV(rd->iv)
			+ getNPreRD(rd, autoPreSelector(rd))
		) - pullRightByte(nextJumbledIV(rd->iv)
			^ getNPreIV(rd->iv, autoPreSelector(rd))
			& nextJumbledIV(rd->iv)
		);
}

byte calcul5(byte baseData, RandData* rd) {
	return
		baseData 
		+ getNPreRD(rd, autoPreSelector(rd))
		^ (
			pullRightByte(
				getNPreIV(rd->iv, autoPreSelector(rd))
				- getNPreRD(rd, autoPreSelector(rd))
			)
			- nextJumbledIV(rd->iv)
			+ pullLeftByte(
				nextRDRSeed(rd)
				+ getNPreIV(rd->iv, autoPreSelector(rd))
			)
			+ mirrorByte(
				nextRDRSeed(rd)
				| getNPreIV(rd->iv, autoPreSelector(rd))
				- baseData
			)
			)
		- slideByte(
			mirrorByte(
				getNPreIV(rd->iv, autoPreSelector(rd))
			)
			+ nextJumbledIV(rd->iv)
			, nextRDRSeed(rd) + getNPreRD(rd, autoPreSelector(rd)) + baseData
		)
		+ nextRDRSeed(rd);
}