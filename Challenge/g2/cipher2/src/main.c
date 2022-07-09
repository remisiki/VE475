#include "twofish.h"
#include "sha256.h"
#include <immintrin.h>

// Read a whole text file into buffer
Buffer readFile(const char* fileName) {

	// Return empty buffer when error
	Buffer buffer = {NULL, 0};

	FILE *fp = fopen(fileName, "r");
	if (fp == NULL) {
		// Open error
		return buffer;
	}

	size_t range = 1;
	size_t char_cnt = 0;
	size_t size = range;
	unsigned char* content = (unsigned char*)malloc(size);
	int c;

	while (((c = fgetc(fp)) != EOF) && (c != '\n')) {
		if (char_cnt >= size - 1) {
			size += range;
			content = (unsigned char*)realloc(content, size);
		}
		content[char_cnt] = (unsigned char)c;
		char_cnt ++;
	}

	fclose(fp);

	buffer.length = size - 1;
	content[size - 1] = '\0';
	buffer.data = content;

	return buffer;

}

int main(int argc, char const *argv[]) {

	uint8_t* p = NULL;
	uint8_t* c = NULL;
	Buffer plainKeyBuffer = {NULL, 0};

	uint8_t cheaterCipher[] = "972f754ec2281ace39611ff73082cf38cdd15f8451a577a1a072236c58907f050aaa8d047223ba0e3aa1a98b5b1e295b5d4c3862bc3441103250e5ae51fe12080debfa2d8715f5f89cb76ef676dab530764f4e712620fa2ed6150eaf523acaab980744b63641417e9f82b9af8a68d70c029f92c1cfc437c9791ff4e1f53da839582a7b9251e239f2af1ab027f30402a9d96cd44b6002913d533cdf97195278c2cf066af4ca5ea943d087f5d0a812d10c8d9c98590711e65a15ffe782aab25ec7c0030b51c72ad01068dba5ef5013cbcae84ef918e6e0f6407e7b47f31775901c4798ed53d92ab764736998d3bd4ead3d1b2f4d079990c77aeb814c6dd65ba8dabc517f7afe9e918414a1ba7a98a572393d23db260aff9844cdcbf80d0c091cffe93015ee36e8c63c0eca9bed7dabb58e949782c19d57278386d2875a35f89c8c04ac5e671f62dbf14d14ef864a1049ea5f3e09c2ce01ebeab27c1240cb64c9b339b80f5f7e11b077cf947b71b5d193639e5ba2bc8619ce966dbf79e33eb808bc864cc86367bbf9a3edb4013f545d8b2fcc93c4316b41a307275c3e41595efeeda4fcc51e77e3578d9fc33bf58f77359f433eec6552a23ceed384c207a0dc00090f5a0c1a84bed788e0ae1cea10b990f5";
	uint8_t defaultKey[] = "7aefb9e392f2f65abc3b3151bc20fa67d3db52179b7e535999dde7e845b0a6bf9f5fa23151988cdd4bd5f90dd59f7b1344ac8cfc81c2961f910493ec0effd5f4e9f91a0381dadcdac9cb765936899dd3169a54d92746a25084634081c28e20532e5870dc644ae8024e946ae7719b5017e8e21e2e856eb2e8ae33aa3d54fe6bac6e4711b93011bb7ea5256517c1b62ea9984cd81dba8a569beb0a612ae11be1c59cfd589e190786b66c9d7722cb20bb17f8d8d283de3b418ee153";

	for (int i = 0; i < argc; i ++) {
		if (strcmp(argv[i], "--generate") == 0) {
			uint32_t seed;
			_rdrand32_step(&seed);
			srand(seed);
			uint8_t length = (uint8_t)(rand() & 0xff);
			while (length --) {
				printf("%x", rand() & 0xff);
			}
			printf("\n");
		} else if (strcmp(argv[i], "--key") == 0) {
			if (argv[i + 1] == NULL) {
				printf("No key file.\n");
				goto EXIT;
			} else {
				plainKeyBuffer = readFile(argv[i + 1]);
				if (plainKeyBuffer.data == NULL) {
					printf("Bad key file.\n");
					goto EXIT;
				}
			}
		} else if (strcmp(argv[i], "--encrypt") == 0) {
			if ((argv[i + 1] == NULL) || (strcmp(argv[i + 1], "") == 0)) {
				printf("No encryption data.\n");
				goto EXIT;
			} else {
				size_t argLength = strlen(argv[i + 1]);
				p = malloc(sizeof(uint8_t) * (argLength + 1));
				memcpy(p, (uint8_t*)argv[i + 1], argLength);
				p[argLength] = '\0';
			}
		} else if (strcmp(argv[i], "--decrypt") == 0) {
			if (argv[i + 1] == NULL) {
				printf("No decryption data.\n");
				goto EXIT;
			} else {
				size_t argLength = strlen(argv[i + 1]);
				c = malloc(sizeof(uint8_t) * (argLength + 1));
				memcpy(c, (uint8_t*)argv[i + 1], argLength);
				c[argLength] = '\0';
			}
		}
	}

	if ((p == NULL) && (c == NULL)) {
		goto EXIT;
	}

	// If no key given, use default key
	if (plainKeyBuffer.data == NULL) {
		// Prevent cheating
		if ((c != NULL) && (memcmp(c, cheaterCipher, strlen((char*)cheaterCipher)) == 0)) {
			printf("cheater: it is forbidden to decrypt the challenge ciphertext");
			goto EXIT;
		}
		size_t length = strlen((char*)defaultKey);
		plainKeyBuffer.data = malloc(sizeof(uint8_t) * length);
		memcpy(plainKeyBuffer.data, defaultKey, length);
		plainKeyBuffer.length = length;
	}

	// Generate keys
	uint8_t* k = hashKey(plainKeyBuffer);
	Buffer keyBuffer = {k, 16};
	KeyGenerator keyGenerator = scheduleKeyGenerator(keyBuffer);
	freeBuffer(keyBuffer);
	uint32_t* keys = scheduleKey(keyGenerator);

	// Encryption
	if (p != NULL) {
		Buffer encrypted = twofishEncrypt(p, strlen((char*)p), keys, keyGenerator.s);
		printBuffer(encrypted, 0, 0);
		printf("\n");
		freeBuffer(encrypted);
	}

	// Decryption
	if (c != NULL) {
		Buffer decrypted = twofishDecrypt(c, strlen((char*)c), keys, keyGenerator.s);
		if (decrypted.data != NULL) {
			printBuffer(decrypted, 1, 0);
			printf("\n");
		} else {
			printf("Failed.\n");
		}
		freeBuffer(decrypted);
	}

	freeKeyGenerator(keyGenerator);
	free(keys);
	EXIT:
	freeBuffer(plainKeyBuffer);
	free(p);
	free(c);

	return 0;

}
