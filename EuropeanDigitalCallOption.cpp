#include "EuropeanDigitalCallOption.h"

EuropeanDigitalCallOption::EuropeanDigitalCallOption(double expiry, double strike)
	: EuropeanDigitalOption(expiry, strike) {}

double EuropeanDigitalCallOption::payoff(double z)
{
	if (z >= getStrike())
		return 1.0;
	return 0.0;
}

EuropeanDigitalOption::optionType EuropeanDigitalCallOption::GetOptionType()
{
	return optionType::call;
}
