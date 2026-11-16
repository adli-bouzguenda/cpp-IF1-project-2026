#include "AmericanPutOption.h"


//builds the american put
//input : expiry the expiry date, strike the strike k
//output : nothing
AmericanPutOption::AmericanPutOption(double expiry, double strike) : AmericanOption(expiry, strike)
{
}


//payoff if the put is exercised
//input : z price of the underlying
//output : k minus z if k is above z, else 0
double AmericanPutOption::payoff(double z)
{
	//gain only when the strike is above z
	if (getStrike()>=z)
		return getStrike() - z;
	return 0.0;
}
