#pragma once
#include "AmericanOption.h"

class AmericanCallOption : public AmericanOption
{
public:
	AmericanCallOption(double expiry, double strike); //uses the american option constructor
	double payoff(double z) override; //z minus k if positive, else 0
};
