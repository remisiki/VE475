#include "twofish.h"

uint8_t* hashKey(const Buffer b) {

	Buffer keyHash = sha256Hash(b);
	uint8_t* k = calloc(16, sizeof(uint8_t));

	// XOR left and right part of SHA-256 hash
	for (size_t i = 0; i < 2; i ++) {
		for (size_t j = 0; j < 16; j ++) {
			k[j] ^= keyHash.data[(i << 4) + j];
		}
	}
	freeBuffer(keyHash);

	return k;

}

KeyGenerator scheduleKeyGenerator(Buffer b) {

	size_t k = (b.length >> 3); // Reserved for further implementation of Twofish-192/256
	uint32_t* m = calloc(2*k, sizeof(uint32_t));
	uint32_t* s = calloc(k, sizeof(uint32_t));

	// Calculate M[4]
	for (size_t i = 0; i < 2 * k; i ++) {
		for (size_t j = 0; j < 4; j ++) {
			m[i] += (uint32_t)((b.data[(i << 2) + j] << (8 * j)));
		}
	}

	// Calculate S[2]
	for (size_t i = 0; i < k; i ++) {
		uint8_t _s_i[4] = {0};
		for (size_t j = 0; j < 4; j ++) {
			for (size_t l = 0; l < 8; l ++) {
				_s_i[j] = (uint8_t)((_s_i[j] + dot(RS[(j << 3) + l], b.data[(i << 3) + l])) & 0xff);
			}
			s[i] += (uint32_t)((_s_i[j] << (8 * j)));
		}
	}

	KeyGenerator keyGenerator = {m, s};
	return keyGenerator;

}

uint8_t twofishPermutation(uint8_t x, size_t type) {

	uint8_t a[5];
	uint8_t b[5];

	a[0] = (x >> 4);
	b[0] = x & 0xf;
	a[1] = a[0] ^ b[0];
	b[1] = (uint8_t)(a[0] ^ rotate8(b[0], 1, 1) ^ ((a[0] << 3) & 0xf));
	a[2] = Q[type][0][a[1]];
	b[2] = Q[type][1][b[1]];
	a[3] = a[2] ^ b[2];
	b[3] = (uint8_t)(a[2] ^ rotate8(b[2], 1, 1) ^ ((a[2] << 3) & 0xf));
	a[4] = Q[type][2][a[3]];
	b[4] = Q[type][3][b[3]];

	return ((uint8_t)((b[4] << 4) + a[4]));

}

uint32_t* scheduleKey(KeyGenerator keyGenerator) {

	uint32_t* keys = calloc(40, sizeof(uint32_t));

	for (uint32_t i = 0; i < 20; i ++) {
		uint32_t a = twofishFunctionH(2 * i * RHO, keyGenerator.m, 2);
		uint32_t b = rotate32(twofishFunctionH((2 * i  + 1)* RHO, keyGenerator.m + 2, 2), 0, 8);
		keys[2 * i] = (a + b) & 0xffffffff;
		keys[2 * i + 1] = rotate32((a + 2 * b) & 0xffffffff, 0, 9);
	}

	return keys;

}

uint32_t twofishFunctionH(const uint32_t x, const uint32_t* list, const size_t length) {

	size_t k = length;
	uint8_t y[4];
	uint8_t z[4] = {0};
	uint32_t result = 0;

	// Calculate l[][]
	uint8_t** l = calloc(k, sizeof(uint8_t*));
	for (size_t i = 0; i < k; i ++) {
		l[i] = calloc(4, sizeof(uint8_t));
	}
	for (size_t i = 0; i < k; i ++) {
		for (size_t j = 0; j < 4; j ++) {
			l[i][j] = (uint8_t)((list[i] >> (8 * j)) & 0xff);
		}
	}

	// Calculate y[]
	for (size_t i = 0; i < 4; i ++) {
		y[i] = twofishPermutation(twofishPermutation(twofishPermutation((uint8_t)((x >> (8 * i)) & 0xff), 0) ^ l[1][i], 0) ^ l[0][i], 1);
	}

	// Free l[][]
	for (size_t i = 0; i < k; i ++) {
		free(l[i]);
	}
	free(l);

	// Calculate z[]
	for (size_t i = 0; i < 4; i ++) {
		for (size_t j = 0; j < 4; j ++) {
			z[i] ^= dot(MDS[(i << 2) + j], y[j]);
		}
		result += (uint32_t)z[i] << (8 * i);
	}

	return result;

}

uint32_t twofishFunctionG(const uint32_t x, const uint32_t* s) {
	return twofishFunctionH(x, s, 2);
}

Pair twofishFunctionF(const Pair f_in, const size_t round, const uint32_t* s, const uint32_t* keys) {

	Pair g_out = {twofishFunctionG(f_in.first, s), twofishFunctionG(rotate32(f_in.second, 0, 8), s)};

	Pair result;
	result.first = (((g_out.first + g_out.second) & 0xffffffff) + keys[2 * round + 8]) & 0xffffffff;
	result.second = (((g_out.first + (g_out.second << 1)) & 0xffffffff) + keys[2 * round + 9]) & 0xffffffff;

	return result;

}

Buffer twofishEncrypt(const uint8_t* p, const size_t length, const uint32_t* keys, const uint32_t* s) {

	// Make a copy of p
	uint8_t* plainCopy = malloc(sizeof(uint8_t) * length);
	memcpy(plainCopy, p, length);

	// Hash the whole p
	Buffer plainBuffer = {plainCopy, length};
	Buffer hash = sha256Hash(plainBuffer);

	// Free p copy
	freeBuffer(plainBuffer);

	Buffer cipher = {malloc(0), 0};
	size_t blockNum = (length >> 4) + ((length & 0xf) ? 1 : 0);

	// XOR first two 32-bits words of hash(p) as random seed
	uint32_t seed = charToUint32(hash.data) ^ charToUint32(hash.data + 16);
	srand(seed);

	// Block cipher
	for (size_t i = 0; i < blockNum; i ++) {

		cipher.length += 16;
		cipher.data = realloc(cipher.data, sizeof(uint8_t) * cipher.length);

		uint8_t block[16];
		uint32_t r_in[4];
		uint32_t r_out[4];
		// Copy one block to encrypt
		strncpy((char*)block, (char*)(p + (i << 4)), 16);

		// Whitening
		for (size_t j = 0; j < 4; j ++) {
			r_in[j] = charToUint32(block + (j << 2));
			r_in[j] ^= keys[j];
		}

		// Start 16 rounds
		for (size_t j = 0; j < 16; j ++) {
			r_out[2] = r_in[0];
			r_out[3] = r_in[1];
			Pair f_in = {r_in[0], r_in[1]};
			Pair f_out = twofishFunctionF(f_in, j, s, keys);
			r_out[0] = rotate32(r_in[2] ^ f_out.first, 1, 1);
			r_out[1] = rotate32(r_in[3], 0, 1) ^ f_out.second;
			r_in[0] = r_out[0];
			r_in[1] = r_out[1];
			r_in[2] = r_out[2];
			r_in[3] = r_out[3];
		}

		// Final swap and whitening
		r_out[0] = r_in[2] ^ keys[4];
		r_out[1] = r_in[3] ^ keys[5];
		r_out[2] = r_in[0] ^ keys[6];
		r_out[3] = r_in[1] ^ keys[7];

		// XOR with random
		for (size_t j = 0; j < 4; j ++) {
			r_out[j] ^= ((uint32_t)rand() & 0xffffffff);
		}

		// Append block cipher to the result
		Buffer encrypted = uint32ToBuffer(r_out, 4);
		memcpy(cipher.data + (i << 4), encrypted.data, 16);
		freeBuffer(encrypted);

	}

	// Append hash to the end
	cipher.data = realloc(cipher.data, sizeof(uint8_t) * (cipher.length + 32));
	memcpy(cipher.data + cipher.length, hash.data, 32);
	for (size_t i = 0; i < 32; i ++) {
		// XOR hash with last block
		cipher.data[i + cipher.length] ^= cipher.data[i + cipher.length - 16];
	}
	cipher.length += 32;

	freeBuffer(hash);

	return cipher;

}

Buffer twofishDecrypt(const uint8_t* c, const size_t length, const uint32_t* keys, const uint32_t* s) {

	// Empty buffer is returned when cipher is invalid
	Buffer emptyBuffer = {NULL, 0};

	// Check length not enough (at least one block and one hash value)
	// Or length not divided by 32 (4-bit words = 128 bits)
	if ((length <= 0x40) || (length & 0x1F)) {
		return emptyBuffer;
	}

	// Extratc hash value from the end
	Buffer hash = hexStringToBuffer(c + length - 0x40, 32);
	Buffer lastBlock = hexStringToBuffer(c + length - 0x60, 16);
	for (size_t i = 0; i < 16; i ++) {
		uint8_t t = hash.data[i];
		hash.data[i] ^= lastBlock.data[i];
		lastBlock.data[i] = t;
	}
	for (size_t i = 16; i < 32; i ++) {
		hash.data[i] ^= lastBlock.data[i - 16];
	}
	freeBuffer(lastBlock);

	// XOR first two 32-bits words of hash(p) as random seed
	uint32_t seed = charToUint32(hash.data) ^ charToUint32(hash.data + 16);
	srand(seed);

	Buffer plain = {malloc(0), 0};
	size_t blockNum = ((length - 0x40) >> 5);

	// Block decipher
	for (size_t i = 0; i < blockNum; i ++) {

		plain.length += 16;
		plain.data = realloc(plain.data, sizeof(uint8_t) * plain.length);

		// Copy one block
		Buffer b = hexStringToBuffer(c + (i << 5), 16);
		uint32_t r_in[4];
		uint32_t r_out[4];

		// XOR with random
		for (size_t j = 0; j < 4; j ++) {
			r_out[j] = charToUint32(b.data + (j << 2));
			r_out[j] ^= ((uint32_t)rand() & 0xffffffff);
		}

		freeBuffer(b);

		// De-whitening and swap
		r_in[0] = r_out[2] ^ keys[6];
		r_in[1] = r_out[3] ^ keys[7];
		r_in[2] = r_out[0] ^ keys[4];
		r_in[3] = r_out[1] ^ keys[5];

		// Start 16 rounds
		for (size_t j = 0; j < 16; j ++) {
			r_out[0] = r_in[2];
			r_out[1] = r_in[3];
			Pair f_in = {r_in[2], r_in[3]};
			Pair f_out = twofishFunctionF(f_in, 15 - j, s, keys);
			r_out[2] = rotate32(r_in[0], 0, 1) ^ f_out.first;
			r_out[3] = rotate32((r_in[1] ^ f_out.second), 1, 1);
			r_in[0] = r_out[0];
			r_in[1] = r_out[1];
			r_in[2] = r_out[2];
			r_in[3] = r_out[3];
		}

		// De-whitening
		for (size_t j = 0; j < 4; j ++) {
			r_out[j] ^= keys[j];
		}

		// Append block plain to the result
		Buffer decrypted = uint32ToBuffer(r_out, 4);
		memcpy(plain.data + (i << 4), decrypted.data, 16);
		freeBuffer(decrypted);

	}

	// Delete 0x00 blocks at tail
	while (plain.data[plain.length - 1] == 0x0) {
		plain.length --;
	}

	// Verify hash on decrypted plain
	Buffer decryptedHash = sha256Hash(plain);
	if (memcmp(decryptedHash.data, hash.data, 32) != 0x0) {
		freeBuffer(decryptedHash);
		freeBuffer(hash);
		freeBuffer(plain);
		return emptyBuffer;
	} else {
		freeBuffer(decryptedHash);
		freeBuffer(hash);
		return plain;
	}

}