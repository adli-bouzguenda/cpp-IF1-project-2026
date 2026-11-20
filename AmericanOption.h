#pragma once
#include "Option.h"

class AmericanOption : public Option
{
public:
	AmericanOption(double expiry, double strike); //expiry and strike k
	bool isAmericanOption() override; //always true here

//protected so that we can use it in a derived class
protected:
	double getStrike(); //for the call and the put, not public

//private to block the class from a derived class and an outside class
private:
	double _strike; //strike k
};
