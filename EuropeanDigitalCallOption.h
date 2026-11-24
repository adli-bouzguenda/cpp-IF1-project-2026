#pragma once
#include "EuropeanDigitalOption.h"

// Digital call: pays 1 if the underlying ends at or above the strike
class EuropeanDigitalCallOption : public EuropeanDigitalOption
{
public:
	// Only passes the values to the EuropeanDigitalOption constructor
	EuropeanDigitalCallOption(double expiry, double strike);

	// h(z) = 1 if z >= K, 0 otherwise
	double payoff(double z) override;

	// Returns optionType::call
	optionType GetOptionType() override;
};
