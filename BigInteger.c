#include "BigInteger.h"

extern BigInt* setHex(const char *hex) {
	size_t hexSize = strlen(hex);
	if (hexSize == 0) {
		fprintf(stderr, "Invalid input Hex\n");
		return NULL;
	}
	size_t size = (hexSize + BUFFSIZE - 1) / BUFFSIZE;

	BigInt *bigint = (BigInt*) malloc(size * sizeof(BigInt));
	if (bigint == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		return NULL;
	}

	bigint->digit = (uint64_t*) malloc(size * sizeof(uint64_t));
	if (bigint->digit == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		free(bigint);
		return NULL;
	}

	bigint->chunks = size;

	char **splitedHex = __splitHex(hex, hexSize, size);

	for (size_t i = 0; i < size; i++) {
		char *endptr;
		bigint->digit[i] = strtoull(splitedHex[i], &endptr, 16);
		if (*endptr != '\0') {
			fprintf(stderr, "Conversion failed for string: %s\n", splitedHex[i]);
			free(bigint->digit);
			return NULL;
		}
	}
	return bigint;
}

extern void freeBigInt(BigInt *x) {
	free(x->digit);
}

extern char* getHex(BigInt *x) {
	if (x == NULL || x->digit == NULL) {
		fprintf(stderr, "Invalid BigInt structure or buffers.\n");
		return NULL;
	}

	char *output = (char*) malloc((BUFFSIZE * x->chunks + 1) * sizeof(char));

	if (output == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		return NULL;
	}

	char *result = output;

	for (size_t i = x->chunks - 1; i != -1; i--) {
		snprintf(output, BUFFSIZE + 1, "%016" PRIX64, x->digit[i]);
		output += BUFFSIZE;
	}

	*output = '\0';

	int i = 0;
	while (result[i] == '0') {
		i++;
	}

	strcpy(result, result + i);

	return result;
}

extern void printBigInt(BigInt *x) {
	printf("%s\n", getHex(x));
}

extern void invBigInt(BigInt *x) {
	for (size_t i = 0; i < x->chunks; i++) {
		x->digit[i] = ~x->digit[i];
	}
}

extern void xorBigInt(BigInt *x, const BigInt *y) {
	if (x->chunks != y->chunks) {
		fprintf(stderr, "XOR size mismatch\n");
		return;
	}
	for (size_t i = 0; i < x->chunks; i++) {
		x->digit[i] ^= y->digit[i];
	}
}

extern void orBigInt(BigInt *x, const BigInt *y) {
	if (x->chunks != y->chunks) {
		fprintf(stderr, "OR size mismatch\n");
		return;
	}
	for (size_t i = 0; i < x->chunks; i++) {
		x->digit[i] |= y->digit[i];
	}
}

extern void andBigInt(BigInt *x, const BigInt *y) {
	if (x->chunks != y->chunks) {
		fprintf(stderr, "AND size mismatch\n");
		return;
	}
	for (size_t i = 0; i < x->chunks; i++) {
		x->digit[i] &= y->digit[i];
	}
}

extern void shiftR_BigInt(BigInt *bigint, const int bits) {
	for (size_t i = 0; i < bigint->chunks; i++) {
		bigint->digit[i] >>= bits;
	}
}

extern void shiftL_BigInt(BigInt *bigint, const int bits) {
	for (size_t i = 0; i < bigint->chunks; i++) {
		bigint->digit[i] <<= bits;
	}
}

extern void addBigInt(BigInt *x, const BigInt *y) {
	uint64_t carry = 0;
	for (size_t i = 0; i < x->chunks; i++) {
		uint64_t sum = x->digit[i] + y->digit[i] + carry;

		if (sum < x->digit[i] || sum < y->digit[i]) {
			carry = 1;
		} else {
			carry = 0;
		}

		x->digit[i] = sum;
	}

	if (carry && x->chunks < x->chunks + 1) {
		x->chunks += 1;
		x->digit = (uint64_t*) realloc(x->digit, x->chunks * sizeof(uint64_t));
		if (x->digit == NULL) {
			fprintf(stderr, "Memory allocation failed\n");
			return;
		}
		x->digit[x->chunks] = 1;
	}
}

extern void subBigInt(BigInt *x, const BigInt *y) {
	if (y->chunks > x->chunks
			|| (y->digit[0] > x->digit[0] && x->chunks == y->chunks)) {
		fprintf(stderr, "The second digit is bigger than the first\n");
		return;
	}

	int borrow = 0;
	for (size_t i = 0; i < x->chunks; i++) {
		if (x->digit[i] < y->digit[i] + borrow) {
			x->digit[i] = x->digit[i] + (UINT64_MAX - y->digit[i]) - borrow + 1;
			borrow = 1;
		} else {
			x->digit[i] = x->digit[i] - y->digit[i] - borrow;
			borrow = 0;
		}
	}
}

void modBigInt(BigInt *x, const BigInt *y) {
	for (size_t i = 0; i < x->chunks; i++) {
		x->digit[i] = x->digit[i] % y->digit[i];
	}
}