#include "EuropeanDigitalPutOption.h"

EuropeanDigitalPutOption::EuropeanDigitalPutOption(double expiry, double strike)
	: EuropeanDigitalOption(expiry, strike) {}

double EuropeanDigitalPutOption::payoff(double z)
{
	if (z <= getStrike())
		return 1.0;
	return 0.0;
}

EuropeanDigitalOption::optionType EuropeanDigitalPutOption::GetOptionType()
{
	return optionType::put;
}
