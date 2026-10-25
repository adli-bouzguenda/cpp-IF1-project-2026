#pragma once
#include "EuropeanDigitalOption.h"

// Digital put: pays 1 if the underlying ends at or below the strike
class EuropeanDigitalPutOption : public EuropeanDigitalOption
{
public:
	// Only passes the values to the EuropeanDigitalOption constructor
	EuropeanDigitalPutOption(double expiry, double strike);

	// h(z) = 1 if z <= K, 0 otherwise
	double payoff(double z) override;

	// Returns optionType::put
	optionType GetOptionType() override;
};
