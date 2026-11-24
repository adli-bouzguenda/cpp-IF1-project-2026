#pragma once
#include "Option.h"

/*
   A European digital option: pays 1 (or 0) at expiry, depending on the strike K.
   Designed like EuropeanVanillaOption (same members), but as a separate class:
   a digital option is not a vanilla option, its payoff is completely different.
   Still abstract: EuropeanDigitalCallOption and EuropeanDigitalPutOption
   write payoff and GetOptionType.
*/
class EuropeanDigitalOption : public Option
{
public:
	// "enum class" = a type that can only take the values call or put
	enum class optionType { call, put };

	// Calls the Option constructor for the expiry, then stores the strike.
	// Throws an exception if expiry or strike is negative.
	EuropeanDigitalOption(double expiry, double strike);

	// Each derived class says if it is a call or a put
	virtual optionType GetOptionType() = 0;

protected:
	// Read access to the strike for the derived classes (not public)
	double getStrike();

private:
	double _strike;   // strike K

	// BlackScholesPricer is allowed to read _strike directly
	friend class BlackScholesPricer;
};
