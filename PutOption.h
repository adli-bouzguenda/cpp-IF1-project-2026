#pragma once
#include "EuropeanVanillaOption.h"

// European put: the right to sell the underlying at the strike K at expiry
class PutOption : public EuropeanVanillaOption
{
public:
	// Only passes the values to the EuropeanVanillaOption constructor
	PutOption(double expiry, double strike);

	// h(z) = K - z if K >= z, 0 otherwise
	double payoff(double z) override;

	// Returns optionType::put
	optionType GetOptionType() override;
};
