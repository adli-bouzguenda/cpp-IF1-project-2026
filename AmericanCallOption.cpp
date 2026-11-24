#include "AmericanCallOption.h"


//builds the american call
//input : expiry and strike
//output : nothing
AmericanCallOption::AmericanCallOption(double expiry, double strike) : AmericanOption(expiry, strike)
{
}


//payoff if the call is exercised
//input : z the price of the underlying
//output : z minus k if z is above k, 0 if not
//same payoff as the european call
double AmericanCallOption::payoff(double z)
{
	//gain only when z is above the strike
	if (z>=getStrike())
		return z - getStrike();
	return 0.0;
}
