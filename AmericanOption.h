#pragma once
#include "Option.h"

class AmericanOption : public Option
{
public:
	AmericanOption(double expiry, double strike); //expiry and strike k
	bool isAmericanOption() override; //always true here

protected:
	double getStrike(); //for the call and the put, not public

private:
	double _strike; //strike k
};
