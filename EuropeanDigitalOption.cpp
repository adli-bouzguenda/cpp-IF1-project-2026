#include "EuropeanDigitalOption.h"
#include <stdexcept>

EuropeanDigitalOption::EuropeanDigitalOption(double expiry, double strike)
	: Option(expiry), _strike(strike)
{
	if (expiry < 0)
		throw std::invalid_argument("Expiry must be nonnegative");
	if (strike < 0)
		throw std::invalid_argument("Strike must be nonnegative");
}

double EuropeanDigitalOption::getStrike()
{
	return _strike;
}
