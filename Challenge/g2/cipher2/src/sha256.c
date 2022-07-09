#include "sha256.h"

/*
	Add SHA-256 padding
*/
Buffer sha256Padding(const Buffer b) {

	size_t paddedLength;
	size_t blockNum = (b.length >> 6) + 1;
	size_t restLength = (b.length & 0x3f);

	// Check block length of last one whether enough for separating bit and length bits
	if (restLength < 0x38) {
		paddedLength = (blockNum << 6);
	} else {
		paddedLength = ((blockNum + 1) << 6);
	}

	uint8_t* paddedText = calloc(paddedLength, sizeof(uint8_t));
	memcpy(paddedText, b.data, b.length);
	// Separating bit
	paddedText[b.length] = 0x80;
	// Bit length of message
	size_t bitLength = (b.length << 3);
	for (size_t i = 0; i < 8; i ++) {
		paddedText[paddedLength - i - 1] = ((bitLength >> (i << 3)) & 0xff);
	}

	Buffer paddedBuffer = {paddedText, paddedLength};
	return paddedBuffer;

}

Buffer sha256Hash(const Buffer b) {

	Buffer paddedBuffer = sha256Padding(b);
	size_t blockNum = (paddedBuffer.length >> 6);

	// Copy of h table
	uint32_t hash[8];
	for (size_t i = 0; i < 8; i ++) {
		hash[i] = SHA256_H[i];
	}

	for (size_t i = 0; i < blockNum; i ++) {

		// Create w table
		uint32_t w[64];
		for (size_t j = 0; j < 16; j ++) {
			w[j] = charToUint32(paddedBuffer.data + (i << 6) + (j << 2));
		}
		for (size_t j = 16; j < 64; j ++) {
			uint32_t _s_0 = rotate32(w[j - 15], 1, 7) ^ rotate32(w[j - 15], 1, 18) ^ (w[j - 15] >> 3);
			uint32_t _s_1 = rotate32(w[j - 2], 1, 17) ^ rotate32(w[j - 2], 1, 19) ^ (w[j - 2] >> 10);
			w[j] = (w[j - 16] + _s_0 + w[j - 7] + _s_1) & 0xffffffff;
		}

		// Working h variables
		uint32_t h[8];
		for (size_t j = 0; j < 8; j ++) {
			h[j] = hash[j];
		}

		for (size_t j = 0; j < 64; j ++) {
			uint32_t _s_1 = rotate32(h[4], 1, 6) ^ rotate32(h[4], 1, 11) ^ rotate32(h[4], 1, 25);
			uint32_t _ch = (h[4] & h[5]) ^ (~h[4] & h[6]);
			uint32_t _t_0 = (h[7] + _s_1 + _ch + SHA256_K[j] + w[j]) & 0xffffffff;
			uint32_t _s_0 = rotate32(h[0], 1, 2) ^ rotate32(h[0], 1, 13) ^ rotate32(h[0], 1, 22);
			uint32_t _maj = (h[0] & h[1]) ^ (h[0] & h[2]) ^ (h[1] & h[2]);
			uint32_t _t_1 = (_s_0 + _maj) & 0xffffffff;
			h[7] = h[6];
			h[6] = h[5];
			h[5] = h[4];
			h[4] = (h[3] + _t_0) & 0xffffffff;
			h[3] = h[2];
			h[2] = h[1];
			h[1] = h[0];
			h[0] = (_t_0 + _t_1) & 0xffffffff;
		}

		// Add to hash value
		for (size_t j = 0; j < 8; j ++) {
			hash[j] = (hash[j] + h[j]) & 0xffffffff;
		}

	}

	freeBuffer(paddedBuffer);

	// Return the 8 hash values
	Buffer result = uint32ToBuffer(hash, 8);
	return result;

}