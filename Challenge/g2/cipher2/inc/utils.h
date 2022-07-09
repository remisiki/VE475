#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

/*
	Buffer of 8-bit word array;
	Every buffer should be freed by freeBuffer(), if data is manually allocated.
*/
typedef struct {
	uint8_t* data;
	size_t length;
} Buffer;

/*
	Twofish key generator lists;
	Call freeKeyGenerator() to free it.
*/
typedef struct {
	uint32_t* m;
	uint32_t* s;
} KeyGenerator;

/*
	A pair of 32-bit word
*/
typedef struct {
	uint32_t first;
	uint32_t second;
} Pair;

/*
	Free buffer data
*/
void freeBuffer(Buffer b);

/*
	Free key generator
*/
void freeKeyGenerator(KeyGenerator k);

/*
	Print the buffer.
	REQUIRES: asString = 0 -> print as hex
	          asString = 1 -> print as ASCII string (may break the reminal)
	          lineBreak = 0 -> print in one line
	          lineBreak = 1 -> print in mutiple lines, separated by bytes
*/
void printBuffer(const Buffer b, const int asString, const int lineBreak);

/*
	Dot product in GF(2^8)
*/
uint8_t dot(uint8_t x, uint8_t y);

/*
	Transform hex string to buffer.
	REQUIRES: x must be string in hex;
	          length must be same as length of x
*/
Buffer hexStringToBuffer(const uint8_t* x, const size_t length);

/*
	Transform 32-bit word to buffer.
	REQUIRES: length must be same as length of x
*/
Buffer uint32ToBuffer(const uint32_t* x, const size_t length);

/*
	Transform 4 8-bit word to one 32-bit word.
*/
uint32_t charToUint32(const uint8_t* c);

/*
	Rotate a 8-bit word.
	REQUIRES: direction = 0 -> Rotate left
	          direction = 1 -> Rotate right
	          step be the rotate amount
*/
uint8_t rotate8(uint8_t x, uint8_t direction, uint8_t step);

/*
	Rotate a 32-bit word.
	REQUIRES: direction = 0 -> Rotate left
	          direction = 1 -> Rotate right
	          step be the rotate amount
*/
uint32_t rotate32(uint32_t x, uint32_t direction, uint32_t step);

#endif