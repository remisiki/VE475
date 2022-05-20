#ifndef _EUCLID_H_
#define _EUCLID_H_

#include <gmpxx.h>

class Euclid {

	private:

		// First number
		mpz_class* a;

		// Second number
		mpz_class* b;

	public:

		struct EuclidCoefficient{

			// Greatest common divisor
			mpz_class gcd;

			// Bezout coefficients
			std::pair<mpz_class, mpz_class> bezout;

		};

		/*
			Constructor takes no arguments and automatically
			generates two random 4096-bit number
		*/
		Euclid();

		/*
			Deconstructor
		*/
		~Euclid();

		/*
			Self implemented extended Euclidean algorithm;
			Return a struct with gcd and the Bezout coeffi-
			cients value pair
		*/
		EuclidCoefficient calcExtendEuclid();

		/*
			Find gcd with built-in function gcd() of gmplib
		*/
		mpz_class calcGcd();

		/*
			Return first number
		*/
		mpz_class getFirst();

		/*
			Return second number
		*/
		mpz_class getSecond();

};

#endif