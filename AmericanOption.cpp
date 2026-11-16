#include "AmericanOption.h"


//builds the american option
//input : expiry the date n, strike the strike k
//output : nothing
AmericanOption::AmericanOption(double expiry, double strike) : Option(expiry), _strike(strike)
{
}


//says that the option is american
//input : nothing
//output : true
//used by crr pricer to know if early exercise is possible
bool AmericanOption::isAmericanOption()
{
	return true;
}


//gives the strike to the derived classes
//input : nothing
//output : the strike k
double AmericanOption::getStrike()
{
	return _strike;
}
