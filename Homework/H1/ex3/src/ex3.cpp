#include <iostream>
#include "euclid.h"

int main(/*int argc, char const *argv[]*/) {
	Euclid euclid_gcd = Euclid();
	const auto coeff = euclid_gcd.calcExtendEuclid();
	const auto gcd = euclid_gcd.calcGcd();
	const bool test_match = (gcd == coeff.gcd);
	const char* result_alert = 
		(test_match) ?
		"\033[1;32mResults match!" :
		"\033[1;31mResults does not match!";
	std::cout
		<< "\033[1;33m" << "First Number: " << "\033[0m" << euclid_gcd.getFirst() << "\n\n"
		<< "\033[1;33m" << "Second Number: " << "\033[0m" << euclid_gcd.getSecond() << "\n\n"
		<< "\033[1;33m" << "First Bezout Coefficient: " << "\033[0m" << coeff.bezout.first << "\n\n"
		<< "\033[1;33m" << "Second Bezout Coefficient: " << "\033[0m" << coeff.bezout.second << "\n\n"
		<< "\033[1;33m" << "Gcd calculated by gmplib: " << "\033[0m" << gcd << "\n\n"
		<< "\033[1;33m" << "Gcd calculated by extended eucliean algorithm: " << "\033[0m" << coeff.gcd << "\n\n"
		<< result_alert << "\n\n";
	return 0;
}