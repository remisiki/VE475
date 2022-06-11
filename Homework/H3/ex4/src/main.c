#include "aes.h"

int main(int argc, char const *argv[]) {
	argc --;
	if ((argc == 0) || ((argc == 1) && (strcmp(argv[1], "-a") == 0))) {
		Buffer key = readFile("key", 0);
		if (key.len == 0) {
			printf("\033[1;31mEmpty key file or file not exists, exit.\033[1;0m\n");
			free(key.data);
			return 0;
		}
		else if (key.len != 16) {
			printf("\033[1;31mKey size is %ld, 128 bit expected.\033[1;0m\n", key.len * 8);
			free(key.data);
			return 0;
		}
		Buffer plain_text = readFile("hoge", 1);
		if (plain_text.len == 0) {
			printf("\033[1;31mEmpty input file or file not exists, exit.\033[1;0m\n");
			free(key.data);
			free(plain_text.data);
			return 0;
		}
		printf("\033[1;36mEncryption\033[1;0m\n");
		encryptAES128(plain_text, key);
		free(plain_text.data);

		Buffer cipher_text = readFile("encrypted", 0);
		if (cipher_text.len == 0) {
			printf("\033[1;31mEmpty encrypted file or file not exists, exit.\033[1;0m\n");
			free(key.data);
			free(cipher_text.data);
			return 0;
		}
		printf("\033[1;36mDecryption\033[1;0m\n");
		decryptAES128(cipher_text, key);
		free(cipher_text.data);

		free(key.data);
	}
	else if (argc == 1) {
		if (strcmp(argv[1], "-e") == 0) {
			Buffer key = readFile("key", 0);
			if (key.len == 0) {
				printf("\033[1;31mEmpty key file or file not exists, exit.\033[1;0m\n");
				free(key.data);
				return 0;
			}
			else if (key.len != 16) {
				printf("\033[1;31mKey size is %ld, 128 bit expected.\033[1;0m\n", key.len * 8);
				free(key.data);
				return 0;
			}
			Buffer plain_text = readFile("hoge", 1);
			if (plain_text.len == 0) {
				printf("\033[1;31mEmpty input file or file not exists, exit.\033[1;0m\n");
				free(key.data);
				free(plain_text.data);
				return 0;
			}
			encryptAES128(plain_text, key);
			free(plain_text.data);
			free(key.data);
		}
		else if (strcmp(argv[1], "-d") == 0) {
			Buffer key = readFile("key", 0);
			if (key.len == 0) {
				printf("\033[1;31mEmpty key file or file not exists, exit.\033[1;0m\n");
				free(key.data);
				return 0;
			}
			else if (key.len != 16) {
				printf("\033[1;31mKey size is %ld, 128 bit expected.\033[1;0m\n", key.len * 8);
				free(key.data);
				return 0;
			}
			Buffer cipher_text = readFile("encrypted", 0);
			if (cipher_text.len == 0) {
				printf("\033[1;31mEmpty encrypted file or file not exists, exit.\033[1;0m\n");
				free(key.data);
				free(cipher_text.data);
				return 0;
			}
			decryptAES128(cipher_text, key);
			free(cipher_text.data);
			free(key.data);
		}
		else {
			printf(
				"This is a simple implementation of AES128.\n"
				"Usage: ex4 [options]\n"
				"\n"
				"       Run without options or -a to see a full demo.\n"
				"       Key should be put into ./key as a binary file.\n"
				"\n"
				"-h\tShow this help message.\n"
				"-a\tEncrypt ./hoge and show the decryption.\n"
				"-e\tEncrypt ./hoge.\n"
				"-d\tDecrypt ./encrypted.\n"
			);
		}
	}

	return 0;
}
