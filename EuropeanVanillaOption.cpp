#include "EuropeanVanillaOption.h"
#include <stdexcept>

// ": Option(expiry)" calls the base class constructor, which stores the expiry.
// Then we check the arguments before keeping the strike.
EuropeanVanillaOption::EuropeanVanillaOption(double expiry, double strike)
	: Option(expiry), _strike(strike)
{
	if (expiry < 0)
		throw std::invalid_argument("Expiry must be nonnegative");
	if (strike < 0)
		throw std::invalid_argument("Strike must be nonnegative");
}

double EuropeanVanillaOption::getStrike()
{
	return _strike;
}