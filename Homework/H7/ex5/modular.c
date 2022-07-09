#include <gmp.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>

void mod1(mpz_t m_0, mpz_t d_0, mpz_t m_1, mpz_t d_1, mpz_t n) {

	clock_t start;
	clock_t end;

	start = clock();

	size_t k_0 = mpz_sizeinbase(d_0, 2);
	size_t k_1 = mpz_sizeinbase(d_1, 2);

	mpz_t power_0;
	mpz_t power_1;
	mpz_init_set_ui(power_0, 1);
	mpz_init_set_ui(power_1, 1);

	for (size_t i = k_0; i > 0; i --) {
		mpz_mul(power_0, power_0, power_0);
		mpz_mod(power_0, power_0, n);
		int is_high_0 = mpz_tstbit(d_0, i - 1);
		if (is_high_0) {
			mpz_mul(power_0, power_0, m_0);;
			mpz_mod(power_0, power_0, n);
		}
	}

	for (size_t i = k_1; i > 0; i --) {
		mpz_mul(power_1, power_1, power_1);
		mpz_mod(power_1, power_1, n);
		int is_high_1 = mpz_tstbit(d_1, i - 1);
		if (is_high_1) {
			mpz_mul(power_1, power_1, m_1);;
			mpz_mod(power_1, power_1, n);
		}
	}

	mpz_mul(power_0, power_0, power_1);
	mpz_mod(power_0, power_0, n);
	gmp_printf("Normal modular result: %Zd\n", power_0);

	end = clock();
	printf("Normal modular time: %fs\n", (double)(end - start) / CLOCKS_PER_SEC);

	mpz_clear(power_0);
	mpz_clear(power_1);

}

void mod2(mpz_t m_0, mpz_t d_0, mpz_t m_1, mpz_t d_1, mpz_t n, mpz_t m_product) {

	clock_t start;
	clock_t end;

	start = clock();

	size_t k_0 = mpz_sizeinbase(d_0, 2);
	size_t k_1 = mpz_sizeinbase(d_1, 2);
	size_t k = (k_0 > k_1) ? k_0 : k_1;

	mpz_t power;
	mpz_init_set_ui(power, 1);

	for (size_t i = k; i > 0; i --) {
		mpz_mul(power, power, power);
		mpz_mod(power, power, n);
		int is_high_0 = mpz_tstbit(d_0, i - 1);
		int is_high_1 = mpz_tstbit(d_1, i - 1);
		if (is_high_0 && is_high_1) {
			mpz_mul(power, power, m_product);;
		} else if (is_high_0) {
			mpz_mul(power, power, m_0);;
		} else if (is_high_1) {
			mpz_mul(power, power, m_1);;
		}
		mpz_mod(power, power, n);
	}
	gmp_printf("Faster modular result: %Zd\n", power);

	end = clock();
	printf("Faster modular time: %fs\n", (double)(end - start) / CLOCKS_PER_SEC);

	mpz_clear(power);

}

int main(/*int argc, char const *argv[]*/) {

	mpz_t m_0;
	mpz_t d_0;
	mpz_t m_1;
	mpz_t d_1;
	mpz_t n;
	mpz_t m_product;

	mpz_init(m_0);
	mpz_init(d_0);
	mpz_init(m_1);
	mpz_init(d_1);
	mpz_init(n);

	size_t seed = (size_t)time(NULL);
	gmp_randstate_t rstate;
	gmp_randinit_mt(rstate);
	gmp_randseed_ui(rstate, seed);

	size_t rbits = 15360;

	mpz_urandomb(m_0, rstate, rbits);
	mpz_urandomb(d_0, rstate, rbits);
	mpz_urandomb(m_1, rstate, rbits);
	mpz_urandomb(d_1, rstate, rbits);
	mpz_urandomb(n, rstate, rbits);
	mpz_init(m_product);
	mpz_mul(m_product, m_0, m_1);

	mod1(m_0, d_0, m_1, d_1, n);
	mod2(m_0, d_0, m_1, d_1, n, m_product);

	gmp_randclear(rstate);
	mpz_clear(m_0);
	mpz_clear(d_0);
	mpz_clear(m_1);
	mpz_clear(d_1);
	mpz_clear(n);
	mpz_clear(m_product);

	return 0;

}
