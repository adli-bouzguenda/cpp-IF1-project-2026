#include "CallOption.h"

CallOption::CallOption(double expiry, double strike)
	: EuropeanVanillaOption(expiry, strike) {
}

double CallOption::payoff(double z)
{
	double K = getStrike();
	if (z >= K)
		return z - K;   // the holder buys at K something worth z
	return 0.0;         // not exercised
}

EuropeanVanillaOption::optionType CallOption::GetOptionType()
{
	return optionType::call;
}