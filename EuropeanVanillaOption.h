#pragma once
#include "Option.h"

/*
   A European vanilla option: an Option with a strike K.
   Still abstract: it does not define payoff nor GetOptionType,
   CallOption and PutOption will do it.
*/
class EuropeanVanillaOption : public Option
{
public:
	// "enum class" = a type that can only take the values call or put
	enum class optionType { call, put };

	// Calls the Option constructor for the expiry, then stores the strike.
	// Throws an exception if expiry or strike is negative.
	EuropeanVanillaOption(double expiry, double strike);

	// Each derived class says if it is a call or a put
	virtual optionType GetOptionType() = 0;

protected:
	/*
	   protected = not public, but visible to the derived classes.
	   CallOption and PutOption need the strike to compute their payoff,
	   and _strike is private, so they read it through this method.
	   It is not public, so it respects "no additional public members".
	*/
	double getStrike();

private:
	double _strike;   // strike K

	// BlackScholesPricer is allowed to read _strike directly (asked by the project)
	friend class BlackScholesPricer;
};
