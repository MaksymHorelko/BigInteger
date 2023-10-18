#ifndef BigInteger_h
#define BigInteger_h

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <ctype.h>

#define BUFFSIZE 16

/* Main Structure */
typedef struct {
	uint64_t *digit;
	size_t chunks;
} BigInt;

/* Initialization functions */
extern BigInt* setHex(const char *hex);

/* Getter functions */
extern char* getHex(const BigInt *x);

/* Free function */
extern void freeBigInt(BigInt *x);

/* Operations*/
extern void invBigInt(BigInt *x);
extern void xorBigInt(BigInt *x, const BigInt *y);
extern void orBigInt(BigInt *x, const BigInt *y);
extern void andBigInt(BigInt *x, const BigInt *y);

extern void shiftR_BigInt(BigInt *x, const unsigned int bits);
extern void shiftL_BigInt(BigInt *x, const unsigned int bits);

extern void addBigInt(BigInt *x, const BigInt *y);
extern void subBigInt(BigInt *x, const BigInt *y);
extern void modBigInt(BigInt *x, const BigInt *y);

//extern void mulBigInt(BigInt *x, const BigInt *y);
//extern void divBigInt(BigInt* x, const BigInt *y);

/* Other functions*/
extern void printBigInt(const BigInt *x);

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

static void __strToHex(BigInt *x, char **hex, size_t size) {
	for (size_t i = 0; i < size; i++) {
		char *endptr;
		x->digit[i] = strtoull(hex[i], &endptr, BUFFSIZE);
		if (*endptr != '\0') {
			fprintf(stderr, "Conversion failed for string: %s\n", hex[i]);
			freeBigInt(x);
			return;
		}
	}
}

static BigInt* __createBigInt(size_t chunks) {
    BigInt* bigint = (BigInt*)malloc(sizeof(BigInt));
	if (bigint == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		return NULL;
	}

	bigint->digit = (uint64_t*)calloc(chunks, sizeof(uint64_t));
	if (bigint->digit == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		free(bigint);
		return NULL;
	}

	bigint->chunks = chunks;

    return bigint;
}

static void __copyFromTo(const BigInt* from, BigInt* to, unsigned int start, unsigned int end) {
	if(start > end || end > from->chunks) {
		fprintf(stderr, "Invalid indexes\n");
		return;
	}
	if(from == NULL || to == NULL) {
		char *strERROR =
				(from == NULL) ?
						"Invalid BigInt 'from' structure.\n" : "Invalid BigInt 'to' structure.\n";
		fprintf(stderr, strERROR);
		return;
	}
	free(to->digit);
	to->chunks = end - start;
	to->digit = (uint64_t*)malloc(to->chunks * sizeof(uint64_t));
	int k = 0;
	for(size_t i = end; i > start; i--) {
		to->digit[k] = from->digit[i];
		k++;
	}
}

#endif
