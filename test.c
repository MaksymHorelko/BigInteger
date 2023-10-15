#include "BigInteger.h"

int main() {

	char *hex1 =
			"51bf608414ad5726a3c1bec098f77b1b54ffb2787f8d528a74c1d7fde6470ea4";

	char *hex2 =
			"403db8ad88a3932a0b7e8189aed9eeffb8121dfac05c3512fdb396dd73f6331c";

	BigInt *n1 = setHex(hex1);
	BigInt *n2 = setHex(hex2);

	printf("NUM1: ");
	printBigInt(n1);
	printf("NUM2: ");
	printBigInt(n2);
	printf("\n");

	 printf("INV (n1): ");
	 invBigInt(n1);
	 printBigInt(n1);
	 printf("\n");

	 printf("XOR: ");
	 freeBigInt(n1);
	 n1 = setHex(hex1);
	 xorBigInt(n1, n2);
	 printBigInt(n1);
	 printf("\n");

	 printf("OR: ");
	 freeBigInt(n1);
	 n1 = setHex(hex1);
	 orBigInt(n1, n2);
	 printBigInt(n1);
	 printf("\n");

	 printf("AND: ");
	 freeBigInt(n1);
	 n1 = setHex(hex1);
	 andBigInt(n1, n2);
	 printBigInt(n1);
	 printf("\n");

	 printf("shiftL (n1, 10): ");
	 freeBigInt(n1);
	 n1 = setHex(hex1);
	 shiftL_BigInt(n1, 10);
	 printBigInt(n1);
	 printf("\n");

	 printf("shiftR (n1, 10): ");
	 freeBigInt(n1);
	 n1 = setHex(hex1);
	 shiftR_BigInt(n1, 10);
	 printBigInt(n1);

	 printf("\nNEW NUMS!!\n");
	 freeBigInt(n1);
	 freeBigInt(n2);
	 n1 = setHex("36f028580bb02cc8272a9a020f4200e346e276ae664e45ee80745574e2f5ab80");
	 n2 = setHex("70983d692f648185febe6d6fa607630ae68649f7e6fc45b94680096c06e4fadb");
	 printf("NUM1: ");
	 printBigInt(n1);
	 printf("NUM2: ");
	 printBigInt(n2);
	 printf("add: ");
	 addBigInt(n1, n2);
	 printBigInt(n1);

	 printf("\nNEW NUMS!!\n");
	 freeBigInt(n1);
	 freeBigInt(n2);
	 n1 = setHex("33ced2c76b26cae94e162c4c0d2c0ff7c13094b0185a3c122e732d5ba77efebc");
	 n2 = setHex("22e962951cb6cd2ce279ab0e2095825c141d48ef3ca9dabf253e38760b57fe03");
	 printf("NUM1: ");
	 printBigInt(n1);
	 printf("NUM2: ");
	 printBigInt(n2);
	 printf("sub: ");
	 subBigInt(n1, n2);
	 printBigInt(n1);

	printf("\nNEW NUMS!!\n");
	freeBigInt(n1);
	freeBigInt(n2);
	n1 = setHex("7d7deab2affa38154326e96d350deee1");
	n2 = setHex("97f92a75b3faf8939e8e98b96476fd22");
	printf("NUM1: ");
	printBigInt(n1);
	printf("NUM2: ");
	printBigInt(n2);
	printf("mod: ");
	modBigInt(n2, n1);
	printBigInt(n1);

	freeBigInt(n1);
	freeBigInt(n2);

	getchar();

	return EXIT_SUCCESS;
}
