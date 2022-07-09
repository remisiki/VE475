#include "utils.h"

uint8_t xtime(uint8_t b) {
	return (uint8_t)((b << 1) ^ ((b & 0x80) ? 0x1b : 0x00));
}

uint8_t dot(uint8_t x, uint8_t y) {
	size_t mask;
	uint8_t product = 0;
	for (mask = 1; mask; mask <<= 1) {
		if (y & mask) {
			product ^= x;
		}
		x = xtime(x);
	}
	return product;
}

void freeBuffer(Buffer b) {
	free(b.data);
}

void freeKeyGenerator(KeyGenerator k) {
	free(k.m);
	free(k.s);
}

void printBuffer(const Buffer b, const int asString, const int lineBreak) {
	size_t blockNum = (b.length >> 4) + ((b.length & 0xf) ? 1 : 0);
	for (size_t k = 0; k < blockNum; k ++) {
		for (size_t i = 0; i < 4; i ++) {
			for (size_t j = 0; j < 4; j ++) {
				size_t index = (k << 4) + (i << 2) + j;
				if (asString) {
					if (b.data[index] != '\0') {
						printf("%c", b.data[index]);
					}
				} else {
					printf("%02x", b.data[index]);
				}
				if (lineBreak) {
					printf(" ");
				}
			}
			if (lineBreak) {
				printf("\n");
			}
		}
	}
}

uint8_t hexCharToNum(uint8_t c) {
	if (c >= '0' && c <= '9')
		return (uint8_t)(c - '0');
	if (c >= 'A' && c <= 'F')
		return (uint8_t)(c - 'A' + 10);
	if (c >= 'a' && c <= 'f')
		return (uint8_t)(c - 'a' + 10);
	return 0;
}

uint8_t concatChar(const uint8_t a, const uint8_t b) {
	return (uint8_t)((hexCharToNum(a) << 4) + hexCharToNum(b));
}

Buffer hexStringToBuffer(const uint8_t* x, const size_t length) {
	uint8_t* y = malloc(sizeof(uint8_t) * length);
	for (size_t i = 0; i < length; i++) {
		y[i] = concatChar(x[2 * i], x[2 * i + 1]);
	}
	Buffer b = {y, length};
	return b;
}

uint32_t charToUint32(const uint8_t* c) {
	uint32_t number = 0;
	for (size_t i = 0; i < 4; i ++) {
		number += (uint32_t)(c[i] << ((3 - i) << 3));
	}
	return number;
}

Buffer uint32ToBuffer(const uint32_t* x, const size_t length) {
	uint8_t* c = calloc((length << 2), sizeof(uint8_t));
	for (size_t i = 0; i < length; i ++) {
		for (size_t j = 0; j < 4; j ++) {
			c[(i << 2) + j] = (uint8_t)(x[i] >> (8 * (3 - j)));
		}
	}
	Buffer b;
	b.data = c;
	b.length = (length << 2);
	return b;
}

uint8_t rotate8(uint8_t x, uint8_t direction, uint8_t count) {
	if (direction == 0) {
		return ((uint8_t)(x << count) | (uint8_t)(x >> (4 - count))) & 0xf;
	} else {
		return ((uint8_t)(x >> count) | (uint8_t)(x << (4 - count))) & 0xf;
	}
}

uint32_t rotate32(uint32_t x, uint32_t direction, uint32_t count) {
	if (direction == 0) {
		return ((uint32_t)(x << count) | (uint32_t)(x >> (32 - count)));
	} else {
		return ((uint32_t)(x >> count) | (uint32_t)(x << (32 - count)));
	}
}