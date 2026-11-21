#include "Option.h"
#include <stdexcept>
// Constructor: initializes the option's expiry
Option::Option(double expiry) : _expiry(expiry) {}

//Returns the expiry date
double Option::getExpiry()
{
	return _expiry;
}

// Computes the payoff using the last price of the path
double Option::payoffPath(const std::vector<double>& path)
{
	if (path.empty())
		// check if the path is empty
		throw std::invalid_argument("Price path cannot be empty");
	return payoff(path.back());   // European option: only the last price matters
}

// Returns false for all non-asian options
bool Option::isAsianOption()
{
	return false;
}


//says if the option is american
//input : nothing
//output : false, american option overrides it
bool Option::isAmericanOption()
{
	return false;
}
