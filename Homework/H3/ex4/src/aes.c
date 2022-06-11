#include "aes.h"

unsigned char xtime(unsigned char b) {
	return (unsigned char)((b << 1) ^ ((b & 0x80) ? 0x1b : 0x00));
}

unsigned char dot(unsigned char x, unsigned char y) {
	size_t mask;
	unsigned char product = 0;
	for (mask = 1; mask; mask <<= 1) {
		if (y & mask) {
			product ^= x;
		}
		x = xtime(x);
	}
	return product;
}

void swap(unsigned char* a, unsigned char* b) {
	(*a) = (*a) ^ (*b);
	(*b) = (*a) ^ (*b);
	(*a) = (*a) ^ (*b);
}

void transpose(unsigned char* x) {
	swap(x + 1, x + 4);
	swap(x + 2, x + 8);
	swap(x + 3, x + 12);
	swap(x + 6, x + 9);
	swap(x + 7, x + 13);
	swap(x + 11, x + 14);
}

unsigned char** getRoundKeys(const unsigned char* key) {

	unsigned char** round_keys = (unsigned char**)malloc(sizeof(unsigned char*) * (ROUNDS + 1));
	for (size_t i = 0; i < (ROUNDS + 1); i ++) {
		round_keys[i] = (unsigned char*)malloc(sizeof(unsigned char) * 16);
	}

	// First round key is the same as key
	memcpy(round_keys[0], key, sizeof(unsigned char) * 16);
	transpose(round_keys[0]);

	for (size_t r = 1; r <= ROUNDS; r ++) {
		for (size_t i = 0; i < 4; i ++) {
			for (size_t j = 0; j < 4; j ++) {
				size_t index = (i << 2) + j;
				if (j == 0) {
					switch (i) {
						case 0:
							round_keys[r][index] = round_keys[r - 1][index] ^ SBOX[round_keys[r - 1][7]] ^ ROUND_CONST[r];
							break;
						case 1:
							round_keys[r][index] = round_keys[r - 1][index] ^ SBOX[round_keys[r - 1][11]];
							break;
						case 2:
							round_keys[r][index] = round_keys[r - 1][index] ^ SBOX[round_keys[r - 1][15]];
							break;
						case 3:
							round_keys[r][index] = round_keys[r - 1][index] ^ SBOX[round_keys[r - 1][3]];
							break;
					}
				}
				else {
					round_keys[r][index] = round_keys[r - 1][index] ^ round_keys[r][index - 1];
				}
			}
		}
	}
	return round_keys;
}

void subBytes(unsigned char* x) {
	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			size_t index = (i << 2) + j;
			x[index] = SBOX[x[index]];
		}
	}
}

void inverseSubBytes(unsigned char* x) {
	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			size_t index = (i << 2) + j;
			x[index] = INV_SBOX[x[index]];
		}
	}
}

void addRoundKey(unsigned char* x, const unsigned char* key) {
	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			size_t index = (i << 2) + j;
			x[index] ^= key[index];
		}
	}
}

void shiftRows(unsigned char* x) {

	unsigned char tmp1, tmp2;
	tmp1 = x[4];
	tmp2 = x[15];
	// Shift second row to left
	memmove(x + 4, x + 5, sizeof(unsigned char) * 3);
	// Shift fourth row to right
	memmove(x + 13, x + 12, sizeof(unsigned char) * 3);
	x[7] = tmp1;
	x[12] = tmp2;

	// Shift third row by 2
	swap(x + 8, x + 10);
	swap(x + 9, x + 11);

}

void inverseShiftRows(unsigned char* x) {

	unsigned char tmp1, tmp2;
	tmp1 = x[7];
	tmp2 = x[12];
	// Shift second row to right
	memmove(x + 5, x + 4, sizeof(unsigned char) * 3);
	// Shift fourth row to left
	memmove(x + 12, x + 13, sizeof(unsigned char) * 3);
	x[4] = tmp1;
	x[15] = tmp2;

	// Shift third row by 2
	swap(x + 8, x + 10);
	swap(x + 9, x + 11);

}

void mixColumns(unsigned char* x) {
	for (size_t j = 0; j < 4; j ++) {
		unsigned char tmp[4];
		tmp[0] = dot(2, x[j]) ^ dot(3, x[4 + j]) ^ x[8 + j] ^ x[12 + j];
		tmp[1] = x[j] ^ dot(2, x[4 + j]) ^ dot(3, x[8 + j]) ^ x[12 + j];
		tmp[2] = x[j] ^ x[4 + j] ^ dot(2, x[8 + j]) ^ dot(3, x[12 + j]);
		tmp[3] = dot(3, x[j]) ^ x[4 + j] ^ x[8 + j] ^ dot(2, x[12 + j]);
		x[j] = tmp[0];
		x[4 + j] = tmp[1];
		x[8 + j] = tmp[2];
		x[12 + j] = tmp[3];
	}
}

void inverseMixColumns(unsigned char* x) {
	for (size_t j = 0; j < 4; j ++) {
		unsigned char tmp[4];
		tmp[0] = dot(0x0e, x[j]) ^ dot(0x0b, x[4 + j]) ^ dot(0x0d, x[8 + j]) ^ dot(0x09, x[12 + j]);
		tmp[1] = dot(0x09, x[j]) ^ dot(0x0e, x[4 + j]) ^ dot(0x0b, x[8 + j]) ^ dot(0x0d, x[12 + j]);
		tmp[2] = dot(0x0d, x[j]) ^ dot(0x09, x[4 + j]) ^ dot(0x0e, x[8 + j]) ^ dot(0x0b, x[12 + j]);
		tmp[3] = dot(0x0b, x[j]) ^ dot(0x0d, x[4 + j]) ^ dot(0x09, x[8 + j]) ^ dot(0x0e, x[12 + j]);
		x[j] = tmp[0];
		x[4 + j] = tmp[1];
		x[8 + j] = tmp[2];
		x[12 + j] = tmp[3];
	}
}

void printMatrix(const unsigned char* x) {
	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			size_t index = (i << 2) + j;
			printf("%02x ", x[index]);
		}
		printf("\n");
	}
}

void encryptAES128(const Buffer plain_text, const Buffer key) {

	printf("\033[1;33mKey:\033[0m ");
	for (size_t i = 0; i < 16; i ++) {
		printf("%02x", key.data[i]);
	}
	printf("\n");

	printf("\033[1;33mPlain text:\033[0m\n%s\n", plain_text.data);

	// Slice plain text to blocks
	const size_t block_num = plain_text.len / 16;
	unsigned char** round_keys = getRoundKeys(key.data);
	unsigned char** blocks = (unsigned char**)malloc(sizeof(unsigned char*) * block_num);
	for (size_t i = 0; i < block_num; i ++) {
		blocks[i] = (unsigned char*)malloc(sizeof(unsigned char) * 16);
		memcpy(blocks[i], plain_text.data + (i * 16), sizeof(unsigned char) * 16);
	}

	printf("\033[1;33mPlain text (hex):\033[0m\n");
	for (size_t i = 0; i < block_num; i ++) {
		for (size_t j = 0; j < 16; j ++) {
			printf("%02x", blocks[i][j]);
		}
	}
	printf("\n");

	// Start encryption
	for (size_t i = 0; i < block_num; i ++) {
		unsigned char* block = blocks[i];
		transpose(block);
		addRoundKey(block, round_keys[0]);
		for (size_t r = 1; r < ROUNDS; r ++) {
			subBytes(block);
			shiftRows(block);
			mixColumns(block);
			addRoundKey(block, round_keys[r]);
		}
		subBytes(block);
		shiftRows(block);
		addRoundKey(block, round_keys[ROUNDS]);
		transpose(block);
	}

	printf("\033[1;33mCipher text (hex):\033[0m\n");
	for (size_t i = 0; i < block_num; i ++) {
		for (size_t j = 0; j < 16; j ++) {
			printf("%02x", blocks[i][j]);
		}
	}
	printf("\n");

	// Store data
	FILE *fp;
	fp = fopen("encrypted", "wb");
	for (size_t i = 0; i < block_num; i ++) {
		fwrite(blocks[i], 16, 1, fp);
	}
	fclose(fp);

	// Free memory
	for (size_t i = 0; i < (ROUNDS + 1); i ++) {
		free(round_keys[i]);
	}
	free(round_keys);
	for (size_t i = 0; i < block_num; i ++) {
		free(blocks[i]);
	}
	free(blocks);

}

void decryptAES128(const Buffer cipher_text, const Buffer key) {

	printf("\033[1;33mKey:\033[0m ");
	for (size_t i = 0; i < 16; i ++) {
		printf("%02x", key.data[i]);
	}
	printf("\n");

	// Slice cipher text to blocks
	const size_t block_num = cipher_text.len / 16;
	unsigned char** round_keys = getRoundKeys(key.data);
	unsigned char** blocks = (unsigned char**)malloc(sizeof(unsigned char*) * block_num);
	for (size_t i = 0; i < block_num; i ++) {
		blocks[i] = (unsigned char*)malloc(sizeof(unsigned char) * 16);
		memcpy(blocks[i], cipher_text.data + (i * 16), sizeof(unsigned char) * 16);
	}

	printf("\033[1;33mCipher text (hex):\033[0m\n");
	for (size_t i = 0; i < block_num; i ++) {
		for (size_t j = 0; j < 16; j ++) {
			printf("%02x", blocks[i][j]);
		}
	}
	printf("\n");

	// Start decryption
	for (size_t i = 0; i < block_num; i ++) {
		unsigned char* block = blocks[i];
		transpose(block);
		addRoundKey(block, round_keys[ROUNDS]);
		for (size_t r = ROUNDS - 1; r > 0; r --) {
			inverseShiftRows(block);
			inverseSubBytes(block);
			addRoundKey(block, round_keys[r]);
			inverseMixColumns(block);
		}
		inverseShiftRows(block);
		inverseSubBytes(block);
		addRoundKey(block, round_keys[0]);
		transpose(block);
	}

	printf("\033[1;33mPlain text (hex):\033[0m\n");
	for (size_t i = 0; i < block_num; i ++) {
		for (size_t j = 0; j < 16; j ++) {
			printf("%02x", blocks[i][j]);
		}
	}
	printf("\n");

	printf("\033[1;33mPlain text:\033[0m\n");
	for (size_t i = 0; i < block_num; i ++) {
		for (size_t j = 0; j < 16; j ++) {
			printf("%c", blocks[i][j]);
		}
	}
	printf("\n");

	// Store data
	FILE *fp;
	fp = fopen("decrypted", "wb");
	for (size_t i = 0; i < block_num; i ++) {
		for (size_t j = 0; j < 16; j ++) {
			// Trim null characters
			if (blocks[i][j] != '\x00') {
				fwrite(blocks[i] + j, 1, 1, fp);
			}
		}
	}
	fclose(fp);

	// Free memory
	for (size_t i = 0; i < (ROUNDS + 1); i ++) {
		free(round_keys[i]);
	}
	free(round_keys);
	for (size_t i = 0; i < block_num; i ++) {
		free(blocks[i]);
	}
	free(blocks);

}

Buffer readFile(const char* file_name, const int padding) {

	Buffer buffer;

	FILE *fp = fopen(file_name, "r");
	if (fp == NULL) {
		// Open error
		buffer.len = 0;
		buffer.data = NULL;
		return buffer;
	}

	const size_t range = (padding == 1) ? BLOCK_SIZE : 1;
	size_t char_cnt = 0;
	size_t size = range;
	unsigned char* content = (unsigned char*)malloc(size);
	int c;

	while ((c = fgetc(fp)) != EOF) {
		if (char_cnt >= size - 1) {
			size += range;
			content = (unsigned char*)realloc(content, size);
		}
		content[char_cnt] = (unsigned char)c;
		char_cnt ++;
	}

	fclose(fp);

	if (padding == 1) {
		// Pad with null characters
		for (size_t i = char_cnt; i < size - 1; i ++) {
			content[i] = '\0';
		}
		buffer.len = size;
	}
	else {
		buffer.len = size - 1;
	}
	content[size - 1] = '\0';
	buffer.data = content;

	return buffer;

}