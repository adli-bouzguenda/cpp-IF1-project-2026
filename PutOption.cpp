#include "PutOption.h"

PutOption::PutOption(double expiry, double strike)
	: EuropeanVanillaOption(expiry, strike) {
}
//Computes the payoff of a european put option
double PutOption::payoff(double z)
{
	double K = getStrike();
	if (K >= z)
		return K - z;   // the holder sells at K something worth z
	return 0.0;         // not exercised
}

// Returns the option type (put)
EuropeanVanillaOption::optionType PutOption::GetOptionType()
{
	return optionType::put;
}
