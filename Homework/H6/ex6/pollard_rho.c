#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint64_t f(uint64_t x, uint64_t n) {
	return (((x ^ 2) + 1) % n);
}

uint64_t gcd(uint64_t a, uint64_t b) {
	if (b == 0) {
		return a;
	}
	return gcd(b, a % b);
}

uint64_t findFactor(uint64_t n, uint64_t (*f)(uint64_t, uint64_t)) {
	if ((n == 1) || (n == 0)) {
		return n;
	}
	uint64_t a = 2;
	uint64_t b = 2;
	uint64_t d;
	do {
		a = (*f)(a, n);
		b = (*f)((*f)(b, n), n);
		d = gcd(a - b, n);
	} while (d == 1);
	return d;
}

void factorize(uint64_t n) {
	uint64_t factor;
	printf("%ld=", n);
	while (1) {
		factor = findFactor(n, &f);
		if (factor == n) {
			printf("%ld\n", factor);
			break;
		} else {
			printf("%ldx", factor);
			n /= factor;
		}
	}
}

int main(/*int argc, char const *argv[]*/) {
	factorize(737091);
	return 0;
}