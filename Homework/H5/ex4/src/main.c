#include "rsa.h"

int main(/*int argc, char const *argv[]*/) {

	printf("Generating keys...\r");
	RSA_keys keys = rsaGenerate(80);
	printf("Generating keys... Done\n");
	printf("p = ");
	printBuffer(keys.private_key -> p);
	printf("q = ");
	printBuffer(keys.private_key -> q);
	printf("d = ");
	printBuffer(keys.private_key -> d);
	printf("n = ");
	printBuffer(keys.public_key -> n);
	printf("e = ");
	printBuffer(keys.public_key -> e);

	uint8_t* text = (uint8_t*)"EL PSY CONGROO";
	size_t length = strlen((char*)text);
	Buffer plain_text = {text, length};

	printf("Plain text: ");
	printText(plain_text);

	Buffer c = rsaEncrypt(plain_text, *keys.public_key);
	printf("Cipher text: ");
	printBuffer(c);

	Buffer m = rsaDecrypt(c, *keys.private_key);
	printf("Decipher text: ");
	printText(m);

	freeBuffer(c);
	freeBuffer(m);
	freeKeys(keys);

	return 0;

}
