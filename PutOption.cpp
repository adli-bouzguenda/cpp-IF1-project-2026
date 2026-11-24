#include "PutOption.h"

PutOption::PutOption(double expiry, double strike)
	: EuropeanVanillaOption(expiry, strike) {
}

double PutOption::payoff(double z)
{
	double K = getStrike();
	if (K >= z)
		return K - z;   // the holder sells at K something worth z
	return 0.0;         // not exercised
}

EuropeanVanillaOption::optionType PutOption::GetOptionType()
{
	return optionType::put;
}