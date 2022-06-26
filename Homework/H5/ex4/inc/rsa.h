#ifndef _RSA_H_
#define _RSA_H_

#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <string.h>

typedef struct {
	uint8_t* data;
	size_t length;
} Buffer;

typedef struct {
	Buffer n;
	Buffer e;
} RSA_public;

typedef struct {
	Buffer p;
	Buffer q;
	Buffer d;
} RSA_private;

typedef struct {
	RSA_public* public_key;
	RSA_private* private_key;
} RSA_keys;

void freeBuffer(Buffer buffer);

void freeKeys(RSA_keys k);

void printBuffer(Buffer buffer);

void printText(Buffer buffer);

RSA_keys rsaGenerate(size_t security_level);

Buffer rsaEncrypt(Buffer buffer, RSA_public public_key);

Buffer rsaDecrypt(Buffer buffer, RSA_private private_key);

#endif