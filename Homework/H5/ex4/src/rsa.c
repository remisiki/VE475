#include "rsa.h"

size_t getBitsLength(const size_t security_level) {
	size_t r = 16;
	switch (security_level) {
		case 80:
			r = 1024;
			break;
		case 112:
			r = 2048;
			break;
		case 128:
			r = 3072;
			break;
		case 192:
			r = 7680;
			break;
		case 256:
			r = 15360;
			break;
	}
	return r;
}

void freeBuffer(Buffer buffer) {
	free(buffer.data);
}

void freeKeys(RSA_keys k) {
	freeBuffer(k.public_key -> n);
	freeBuffer(k.public_key -> e);
	freeBuffer(k.private_key -> p);
	freeBuffer(k.private_key -> q);
	freeBuffer(k.private_key -> d);
	free(k.public_key);
	free(k.private_key);
}

void printBuffer(Buffer buffer) {
	for (size_t i = 0; i < buffer.length; i ++) {
		printf("%x", buffer.data[i]);
	}
	printf("\n");
}

void printText(Buffer buffer) {
	for (size_t i = 0; i < buffer.length; i ++) {
		printf("%c", buffer.data[i]);
	}
	printf("\n");
}

RSA_keys rsaGenerate(size_t security_level) {

	RSA_public* public_key = malloc(sizeof(RSA_public));
	RSA_private* private_key = malloc(sizeof(RSA_private));
	RSA_keys k = {public_key, private_key};

	size_t seed = (size_t)time(NULL);
	size_t rbits = getBitsLength(security_level);
	gmp_randstate_t rstate;
	mpz_t p;
	mpz_t q;
	mpz_t n;
	mpz_t e;
	mpz_t d;
	mpz_t gcd;
	mpz_t phi;
	mpz_t psub;
	mpz_t qsub;

	gmp_randinit_mt(rstate);
	gmp_randseed_ui(rstate, seed);
	mpz_init(p);
	mpz_init(q);
	mpz_init(n);
	mpz_init(e);
	mpz_init(d);
	mpz_init(gcd);
	mpz_init(phi);
	mpz_init(psub);
	mpz_init(qsub);

	while (mpz_sizeinbase(n, 2) < rbits) {
		while (1) {
			mpz_urandomb(p, rstate, (rbits >> 1));
			if (mpz_probab_prime_p(p, 15)) {
				break;
			}
		}

		while (1) {
			mpz_urandomb(q, rstate, (rbits >> 1));
			if (mpz_probab_prime_p(q, 15)) {
				break;
			}
		}

		mpz_mul(n, p, q);
	}

	mpz_sub_ui(psub, p, 1);
	mpz_sub_ui(qsub, q, 1);
	mpz_mul(phi, psub, qsub);

	while (1) {
		mpz_urandomb(e, rstate, rbits);
		if (mpz_even_p(e)) {
			mpz_add_ui(e, e, 1);
		}
		mpz_gcd(gcd, e, phi);
		if (mpz_cmp_ui(gcd, 1) == 0) {
			mpz_invert(d, e, phi);
			break;
		}
	}

	(public_key -> n).data = mpz_export(NULL, &((public_key -> n).length), 1, 1, 0, 0, n);
	(public_key -> e).data = mpz_export(NULL, &((public_key -> e).length), 1, 1, 0, 0, e);
	(private_key -> p).data = mpz_export(NULL, &((private_key -> p).length), 1, 1, 0, 0, p);
	(private_key -> q).data = mpz_export(NULL, &((private_key -> q).length), 1, 1, 0, 0, q);
	(private_key -> d).data = mpz_export(NULL, &((private_key -> d).length), 1, 1, 0, 0, d);

	gmp_randclear(rstate);
	mpz_clear(p);
	mpz_clear(q);
	mpz_clear(n);
	mpz_clear(e);
	mpz_clear(d);
	mpz_clear(gcd);
	mpz_clear(phi);
	mpz_clear(psub);
	mpz_clear(qsub);

	return k;

}

Buffer rsaEncrypt(Buffer buffer, RSA_public public_key) {

	Buffer r;
	mpz_t m;
	mpz_t c;
	mpz_t n;
	mpz_t e;

	mpz_init(m);
	mpz_init(c);
	mpz_init(n);
	mpz_init(e);

	mpz_import(m, buffer.length, 1, 1, 0, 0, buffer.data);
	mpz_import(n, public_key.n.length, 1, 1, 0, 0, public_key.n.data);
	mpz_import(e, public_key.e.length, 1, 1, 0, 0, public_key.e.data);
	mpz_powm(c, m, e, n);
	r.data = mpz_export(NULL, &(r.length), 1, 1, 0, 0, c);

	mpz_clear(m);
	mpz_clear(c);
	mpz_clear(n);
	mpz_clear(e);

	return r;

}

Buffer rsaDecrypt(Buffer buffer, RSA_private private_key) {

	Buffer r;
	mpz_t m;
	mpz_t c;
	mpz_t p;
	mpz_t q;
	mpz_t d;
	mpz_t n;

	mpz_init(m);
	mpz_init(c);
	mpz_init(p);
	mpz_init(q);
	mpz_init(d);
	mpz_init(n);

	mpz_import(c, buffer.length, 1, 1, 0, 0, buffer.data);
	mpz_import(p, private_key.p.length, 1, 1, 0, 0, private_key.p.data);
	mpz_import(q, private_key.q.length, 1, 1, 0, 0, private_key.q.data);
	mpz_import(d, private_key.d.length, 1, 1, 0, 0, private_key.d.data);
	mpz_mul(n, p, q);
	mpz_powm(m, c, d, n);
	r.data = mpz_export(NULL, &(r.length), 1, 1, 0, 0, m);

	mpz_clear(m);
	mpz_clear(c);
	mpz_clear(p);
	mpz_clear(q);
	mpz_clear(d);
	mpz_clear(n);

	return r;

}