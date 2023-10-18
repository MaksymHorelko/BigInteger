#include "BigInteger.h"

BigInt* setHex(const char *hex) {
	size_t hexSize = strlen(hex);
	if (hexSize == 0 || strncmp(hex, "0x", 2) == 0) {
		fprintf(stderr, "Invalid input Hex\n");
		return NULL;
	}
	for (size_t i = 0; i < hexSize; i++) {
		if (!isxdigit(hex[i])) {
			fprintf(stderr, "Invalid input Hex\n");
			return NULL;
		}
	}

	size_t size = (hexSize + BUFFSIZE - 1) / BUFFSIZE;
	BigInt *bigint = __createBigInt(size);
	if(bigint == NULL) return NULL;

	char **splitedHex = __splitHex(hex, hexSize, size);
	if (splitedHex == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		freeBigInt(bigint);
		return NULL;
	}
	__strToHex(bigint, splitedHex, size);

	return bigint;
}

void freeBigInt(BigInt *x) {
	free(x->digit);
	free(x);
}

char* getHex(const BigInt *x) {
	if (x == NULL || x->digit == NULL) {
		fprintf(stderr, "Invalid BigInt structure or buffer.\n");
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

void printBigInt(const BigInt *x) {
	if (x == NULL) {
		fprintf(stderr, "Invalid BigInt structure.\n");
		return;
	}
	printf("%s\n", getHex(x));
}

void invBigInt(BigInt *x) {
	if (x == NULL) {
		fprintf(stderr, "Invalid BigInt structure.\n");
		return;
	}
	for (size_t i = 0; i < x->chunks; i++) {
		x->digit[i] = ~x->digit[i];
	}
}

void xorBigInt(BigInt *x, const BigInt *y) {
	if (x == NULL || y == NULL) {
		if (x == NULL || x->digit == NULL) {
			char *strERROR =
					(x == NULL) ?
							"Invalid BigInt 'x' structure.\n" :
							"Invalid BigInt 'y' structure.\n";
			fprintf(stderr, strERROR);
			return;
		}
	}
	if (x->chunks != y->chunks) {
		fprintf(stderr, "XOR size mismatch\n");
		return;
	}

	for (size_t i = 0; i < x->chunks; i++) {
		x->digit[i] ^= y->digit[i];
	}
}

void orBigInt(BigInt *x, const BigInt *y) {
	if (x == NULL || y == NULL) {
		if (x == NULL || x->digit == NULL) {
			char *strERROR =
					(x == NULL) ?
							"Invalid BigInt 'x' structure.\n" :
							"Invalid BigInt 'y' structure.\n";
			fprintf(stderr, strERROR);
			return;
		}
	}
	if (x->chunks != y->chunks) {
		fprintf(stderr, "OR size mismatch\n");
		return;
	}
	for (size_t i = 0; i < x->chunks; i++) {
		x->digit[i] |= y->digit[i];
	}
}

void andBigInt(BigInt *x, const BigInt *y) {
	if (x == NULL || y == NULL) {
		if (x == NULL || x->digit == NULL) {
			char *strERROR =
					(x == NULL) ?
							"Invalid BigInt 'x' structure.\n" :
							"Invalid BigInt 'y' structure.\n";
			fprintf(stderr, strERROR);
			return;
		}
	}
	if (x->chunks != y->chunks) {
		fprintf(stderr, "AND size mismatch\n");
		return;
	}
	for (size_t i = 0; i < x->chunks; i++) {
		x->digit[i] &= y->digit[i];
	}
}

void shiftR_BigInt(BigInt *x, const unsigned int bits) {
	if (x == NULL) {
		fprintf(stderr, "Invalid BigInt structure.\n");
		return;
	}
	if (bits == 0) {
		fprintf(stderr, "Invalid number of bits.\n");
		return;
	}

	for (size_t i = 0; i < x->chunks; i++) {
		x->digit[i] >>= bits;
	}
}

void shiftL_BigInt(BigInt *x, const unsigned int bits) {
	if (x == NULL) {
		char *strERROR = "Invalid BigInt structure.\n";
		fprintf(stderr, strERROR);
		return;
	}
	if (bits == 0) {
		fprintf(stderr, "Invalid number of bits.\n");
		return;
	}

	for (size_t i = 0; i < x->chunks; i++) {
		x->digit[i] <<= bits;
	}
}

void addBigInt(BigInt *x, const BigInt *y) {
	if (x == NULL || y == NULL) {
		if (x == NULL || x->digit == NULL) {
			char *strERROR =
					(x == NULL) ?
							"Invalid BigInt 'x' structure.\n" :
							"Invalid BigInt 'y' structure.\n";
			fprintf(stderr, strERROR);
			return;
		}
	}
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

void subBigInt(BigInt *x, const BigInt *y) {
	if (x == NULL || y == NULL) {
		if (x == NULL || x->digit == NULL) {
			char *strERROR =
					(x == NULL) ?
							"Invalid BigInt 'x' structure.\n" :
							"Invalid BigInt 'y' structure.\n";
			fprintf(stderr, strERROR);
			return;
		}
	}
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
	if (x == NULL || y == NULL) {
		if (x == NULL || x->digit == NULL) {
			char *strERROR =
					(x == NULL) ?
							"Invalid BigInt 'x' structure.\n" :
							"Invalid BigInt 'y' structure.\n";
			fprintf(stderr, strERROR);
			return;
		}
	}
	for (size_t i = 0; i < x->chunks; i++) {
		x->digit[i] = x->digit[i] % y->digit[i];
	}
}
