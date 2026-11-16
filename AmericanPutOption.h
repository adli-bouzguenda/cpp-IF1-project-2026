#pragma once
#include "AmericanOption.h"

class AmericanPutOption : public AmericanOption
{
public:
	AmericanPutOption(double expiry, double strike); //uses the american option constructor
	double payoff(double z) override; //k minus z if positive, else 0
};
