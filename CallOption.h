#pragma once
#include "EuropeanVanillaOption.h"

// European call: the right to buy the underlying at the strike K at expiry
class CallOption : public EuropeanVanillaOption
{
public:
	// Only passes the values to the EuropeanVanillaOption constructor
	CallOption(double expiry, double strike);

	// h(z) = z - K if z >= K, 0 otherwise
	double payoff(double z) override;

	// Returns optionType::call
	optionType GetOptionType() override;
};
