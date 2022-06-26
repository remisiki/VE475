#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
	/*
		Representing a rational number a/b
	*/
	size_t a;
	size_t b;
} Rational;

/*
	Verify whether a key is correct.
	REQUIRES: public_key = Rational{e, n}
	          private_key = Rational{k, d}
	RETURNS:  Factorized pair {p, q}, p = q = 0 if not found
*/
Rational verifyKey(Rational public_key, Rational private_key) {
	Rational r = {0, 0};
	if ((private_key.a!=0) && ((public_key.a * private_key.b - 1) % private_key.a == 0)) {
		size_t phi = (public_key.a * private_key.b - 1) / private_key.a;
		size_t s = public_key.b - phi + 1;
		int delta = (int)(s * s - 4 * public_key.b);
		if (delta >= 0) {
			size_t t = (size_t)floor(sqrt(delta));
			if ((t * t == (size_t)delta) && !((s + t) % 2)) {
				r.a = (s - t) / 2;
				r.b = (s + t) / 2;
			}
		}
	}
	return r;
}

/*
	Calculate the continued fraction form of x.
	RETURNS: Array of coefficients
*/
size_t* toContinuedFraction(Rational x) {
	size_t length = 1;
	size_t* q = malloc(sizeof(size_t) * 2);
	while (x.a != 0) {
		size_t r = x.b % x.a;
		q[length++] = x.b / x.a;
		q = realloc(q, sizeof(size_t) * (length + 1));
		x.b = x.a;
		x.a = r;
	}
	q[0] = length;
	return q;
}

/*
	Estimate the rational of fraction.
*/
Rational toRational(const size_t* q) {
	size_t n_0 = q[1];
	size_t n_1 = q[1] * q[2] + 1;
	size_t d_0 = 1;
	size_t d_1 = q[2];
	for (size_t i = 3; i < q[0]; i ++) {
		size_t n_i = q[i] * n_1 + n_0;
		size_t d_i = q[i] * d_1 + d_0;
		n_0 = n_1;
		n_1 = n_i;
		d_0 = d_1;
		d_1 = d_i;
	}
	Rational r = {d_1, n_1};
	return r;
}

/*
	Break RSA given public keys.
	REQUIRES: x = Rational{e, n}
*/
void breakRSA(Rational x) {
	size_t* q = toContinuedFraction(x);
	size_t length = q[0];
	Rational r = {0, 0};
	Rational f = {0, 0};
	for (size_t i = 0; i < length - 1; i ++) {
		q[0] = i + 2;
		Rational c = toRational(q);
		if (i % 2) {
			q[q[0] - 1] ++;
			Rational d = toRational(q);
			if ((c.a == d.a) && (c.b == d.b)) {
				f = verifyKey(x, c);
				if (f.a != 0) {
					r = c;
					break;
				}
			}
			q[q[0] - 1] --;
		}
		else {
			Rational d = toRational(q);
			if ((c.a == d.a) && (c.b == d.b)) {
				f = verifyKey(x, c);
				if (f.a != 0) {
					r = c;
					break;
				}
			}
		}
	}
	free(q);
	if (r.a != 0) {
		printf("Key found!\n");
		printf("p = %ld\n", f.a);
		printf("q = %ld\n", f.b);
		printf("k = %ld\n", r.a);
		printf("d = %ld\n", r.b);
	}
	else {
		printf("Failed.\n");
	}
}

int main(/*int argc, char const *argv[]*/) {
	Rational x = {77537081, 317940011};
	breakRSA(x);
	return 0;
}