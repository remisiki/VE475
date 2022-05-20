#include <time.h>
#include "euclid.h"

Euclid::Euclid() {
	gmp_randclass rand_bits (gmp_randinit_default);
	rand_bits.seed(time(0));
	this -> a = new mpz_class(rand_bits.get_z_bits(4096));
	this -> b = new mpz_class(rand_bits.get_z_bits(4096));
}

Euclid::~Euclid(){
	delete this -> a;
	delete this -> b;
}

Euclid::EuclidCoefficient Euclid::calcExtendEuclid() {
	mpz_class r0 = *(this -> b);
	mpz_class r1 = *(this -> a);
	mpz_class s0 = 0;
	mpz_class s1 = 1;
	mpz_class t0 = 1;
	mpz_class t1 = 0;
	while (r0 != 0) {
		mpz_class q = r1 / r0;
		mpz_class tmp = r1;
		r1 = r0;
		r0 = tmp - q * r0;
		tmp = s1;
		s1 = s0;
		s0 = tmp - q * s0;
		tmp = t1;
		t1 = t0;
		t0 = tmp - q * t0;
	}
	EuclidCoefficient coeff;
	coeff.gcd = r1;
	coeff.bezout = std::make_pair(s1, t1);
	return coeff;
}

mpz_class Euclid::calcGcd() {
	return gcd(*(this -> a), *(this -> b));
}

mpz_class Euclid::getFirst() {
	return *(this -> a);
}

mpz_class Euclid::getSecond() {
	return *(this -> b);
}
