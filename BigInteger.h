#ifndef BigInteger_h
#define BigInteger_h

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>

#define BUFFSIZE 16

/* Main Structure */
typedef struct {
	uint64_t *digit;
	size_t chunks;
} BigInt;

/* Initialization functions */
extern BigInt* setHex(const char *hex);

/* Getter functions */
extern char* getHex(BigInt *x);

/* Free function */
extern void freeBigInt(BigInt *x);

/* Operations*/
extern void invBigInt(BigInt *x);
extern void xorBigInt(BigInt *x, const BigInt *y);
extern void orBigInt(BigInt *x, const BigInt *y);
extern void andBigInt(BigInt *x, const BigInt *y);
extern void shiftR_BigInt(BigInt *x, const int bits);
extern void shiftL_BigInt(BigInt *x, const int bits);

extern void addBigInt(BigInt *x, const BigInt *y);
extern void subBigInt(BigInt *x, const BigInt *y);
extern void modBigInt(BigInt *x, const BigInt *y);

/* Other functions*/
extern void printBigInt(BigInt *x);

static char** __splitHex(const char *input, size_t length, size_t chunks) {
	char **subStrings = (char**) malloc(chunks * sizeof(char*));
	if (subStrings == NULL) {
		return NULL;
	}
	for (int i = 0; i < chunks; i++) {
		int startIndex = length - (i + 1) * BUFFSIZE;
		int subStringLength = BUFFSIZE;
		if (startIndex < 0) {
			startIndex = 0;
			subStringLength = length - i * BUFFSIZE;
		}
		subStrings[i] = (char*) malloc(subStringLength + 3);
		strcpy(subStrings[i], "0x");
		strncat(subStrings[i], input + startIndex, subStringLength);
		subStrings[i][BUFFSIZE + 2] = '\0';
	}
	return subStrings;
}

#endif
